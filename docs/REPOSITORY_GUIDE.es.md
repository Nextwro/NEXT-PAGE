# Organización del repositorio NEXT-PAGE

**[🇬🇧 English version](./REPOSITORY_GUIDE.md) · [Volver al README](../README.md#es)**

Esta guía refleja la revisión de archivos del **9 de noviembre de 2026** y propone mejorar el repositorio sin poner en riesgo el trabajo que ya ha realizado el equipo.

## 1. Inventario comprobado

| Carpeta actual | Contenido encontrado | Mejora sugerida |
|---|---|---|
| `3D DESIGN/` | Modelos STL del marcapáginas, merchandising y juego del abecedario; imágenes de llavero y estación | Separar modelos y vistas previas por dispositivo |
| `DOCUMENTS/` | Informe WRO, guía de estilo, guion y un segundo README | Mantener la documentación y estudiar la duplicación del README |
| `ELECTRIC  DIAGRAM AND CODING/CODING/` | Tres **capturas PNG de programación**: `CODE 1.PNG`, `CODE 2.PNG`, `CODE 3.PNG` | Incorporar los archivos fuente originales `.ino` y `.py` |
| `ELECTRIC  DIAGRAM AND CODING/FRITZING/` | Esquemas PNG del marcapáginas, estación base, pantalla y cargador inalámbrico | Añadir tablas de pines, librerías y versiones |
| `ELECTRIC  DIAGRAM AND CODING/SKETCHES/` | Imágenes de componentes y medidas | Ordenar por parte del prototipo |
| `IMAGES/` | APP, ENTREPRENEURSHIP, NEXT, PROBLEMS, RRSS y TEAM PHOTOS | Conservar fotos, diseños y recursos originales |

### Recursos gráficos verificados

- Banner oficial: `IMAGES/NEXT/BANNER NEXT .png`
- Logo: `IMAGES/NEXT/NEXT logo.png`
- Blanca: `IMAGES/TEAM PHOTOS/BLANCA.jpg`
- Jimena: `IMAGES/TEAM PHOTOS/JIMENA.jpg`
- Mario: `IMAGES/TEAM PHOTOS/MARIO 2.jpg`

El README principal utiliza ahora el banner y las tres fotografías individuales del equipo con las responsabilidades confirmadas por sus integrantes.

## 2. Estructura que queremos alcanzar

```text
NEXT-PAGE/
├── README.md
├── bookmark/
│   ├── firmware/
│   └── hardware/
├── station/
│   ├── next_mmwave_uart.py
│   ├── next_station_api.py
│   ├── static/
│   └── requirements.txt
├── mobile-app/
├── 3d-design/
├── docs/
│   ├── installation/
│   ├── electronics/
│   └── testing/
├── assets/
└── .gitignore
```

La estructura anterior es una **propuesta futura**: no significa que esos archivos ya se encuentren en el repositorio. Los originales se conservan hasta realizar una migración revisada.

## 3. Plan de reorganización

1. **Portada y presentación.** Mantener el README en español e inglés con las funciones reales del proyecto y sus limitaciones.
2. **Documentación bilingüe.** Preparar versiones `.es.md` y `.md` de las guías técnicas para que el equipo y el jurado internacional puedan consultarlas.
3. **Código editable.** Obtener de la estación 1 y del marcapáginas los programas que realmente se han probado. No utilizar código reconstruido sin validación.
4. **Hardware.** Añadir pinouts, diagramas, componentes, instrucciones de montaje y versiones de bibliotecas.
5. **Ordenar carpetas.** Mover o renombrar documentos solo después de comprobar propietarios, enlaces y referencias.
6. **Reproducibilidad.** Probar de principio a fin la estación base 2 y documentar resultados.
7. **Versión para competición.** Etiquetar en GitHub la versión definitiva que haya sido probada en el hardware.

## 4. Buenas prácticas

- Distinguir claramente entre **funcionando**, **probado**, **experimental** y **planificado**.
- No afirmar que el radar mide directamente emociones, concentración o disfrute lector.
- No publicar contraseñas, tokens de Spotify, credenciales, entornos `.venv` ni archivos JSON temporales.
- Subir código editable, no únicamente capturas de pantalla.
- Comprobar permisos y licencias de imágenes, modelos 3D y software.
- Mantener guías en español e inglés de los componentes importantes.

## 5. Reparto técnico del equipo

| Integrante | Área principal |
|---|---|
| **Blanca Cascales** | Electrónica y programación |
| **Jimena Martínez** | Diseño y emprendimiento |
| **Mario Sánchez** | Comunicación y documentación |

Las responsabilidades son complementarias y no implican una jerarquía entre integrantes.
