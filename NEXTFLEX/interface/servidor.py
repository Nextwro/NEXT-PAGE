from pathlib import Path
import sqlite3, threading, time, csv, io
import os
from flask import Flask, jsonify, render_template_string, Response
import serial

# Puerto serie configurable; COM65 es el puerto utilizado en el prototipo.
PORT = os.environ.get('NEXTFLEX_PORT', 'COM65')
BAUD = 115200
DB = Path(__file__).with_name('nextflex.db')
app = Flask(__name__)
status = 'Connecting...'

# Abre una conexion independiente a SQLite para cada operacion.
def connect():
    return sqlite3.connect(DB, timeout=10)

# Crea la tabla si no existe, sin borrar partidas anteriores.
with connect() as db:
    db.execute('''CREATE TABLE IF NOT EXISTS games (
        id INTEGER PRIMARY KEY, played_at TEXT DEFAULT CURRENT_TIMESTAMP,
        player TEXT NOT NULL, country TEXT NOT NULL, hits INTEGER,
        misses INTEGER, errors INTEGER, average_us INTEGER)''')

# Valida el mensaje CSV del firmware y registra una partida.
def save_line(line):
    if not line.startswith('CSV,'):
        return False
    fields = line.split(',')
    if len(fields) != 7:
        raise ValueError('Invalid CSV length')
    _, player, country, h, m, e, a = fields
    h, m, e, a = map(int, (h,m,e,a))
    if not player or len(player)>12 or len(country)!=2 or not country.isalpha() or not (0<=h<=4 and h+m==4 and 0<=e<=11 and 0<=a<1000000 and (h>0 or a==0)):
        raise ValueError('Invalid game data')
    with connect() as db:
        db.execute('INSERT INTO games(player,country,hits,misses,errors,average_us) VALUES (?,?,?,?,?,?)',(player,country.upper(),h,m,e,a))
    return True

# Hilo de recepcion USB; reintenta la conexion si se desconecta.
def reader():
    global status
    while True:
        try:
            with serial.Serial(PORT, BAUD, timeout=1) as port:
                status = 'Connected: '+PORT
                print(status, flush=True)
                pending = bytearray()
                while True:
                    pending.extend(port.read_until(b'\n', 1024))
                    if len(pending)>4096:
                        pending.clear()
                    if not pending.endswith(b'\n'):
                        continue
                    line = pending.decode('utf-8', errors='replace').strip()
                    pending.clear()
                    if line:
                        print(line, flush=True)
                        try:
                            save_line(line)
                        except (ValueError, sqlite3.Error) as exc:
                            print('Not saved:', exc, flush=True)
        except (serial.SerialException, OSError) as exc:
            status = 'Disconnected: '+PORT
            print(status, exc, flush=True)
            time.sleep(3)

# API local: ordena por aciertos, errores y tiempo medio.
@app.get('/api/results')
def results():
    with connect() as db:
        db.row_factory = sqlite3.Row
        rows = db.execute('SELECT * FROM games ORDER BY hits DESC, errors ASC, average_us ASC, id ASC').fetchall()
    return jsonify(status=status, games=[dict(r) for r in rows])

# Exportacion de todas las partidas; protege formulas de hojas de calculo.
@app.get('/export.csv')
def export():
    with connect() as db:
        cur = db.execute('SELECT * FROM games ORDER BY id')
        out = io.StringIO()
        w = csv.writer(out)
        w.writerow([c[0] for c in cur.description])
        for row in cur:
            w.writerow([("'"+v if isinstance(v,str) and v.startswith(('=','+','-','@')) else v) for v in row])
    return Response('\ufeff'+out.getvalue(), mimetype='text/csv', headers={'Content-Disposition':'attachment; filename=nextflex.csv'})

# Interfaz HTML, CSS, SVG y JavaScript autocontenida.
PAGE = Path(__file__).with_name('ranking.html').read_text(encoding='utf-8')

# Entrega HTML literal para evitar interpretar CSS como plantilla Jinja.
@app.get('/')
def home():
    return Response(PAGE, mimetype="text/html")

# Arranque local: no expone el servidor a otras maquinas de la red.
if __name__ == '__main__':
    threading.Thread(target=reader, daemon=True).start()
    app.run(host='127.0.0.1', port=5000, debug=False, use_reloader=False)
