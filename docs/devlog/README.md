# Devlog del fork de CaesarIA

Diario técnico por áreas. Cada archivo tiene una sección "Pendientes" (lo que sigue
abierto) y un "Historial" con las entradas más recientes primero. Antes de tocar un
área, lee su archivo.

| Área | Último cambio | Pendientes | Estado |
|---|---|---|---|
| [Compilación y herramientas](compilacion-y-herramientas.md) | 2026-10-26 | 1 | Compila en Windows x64 con MinGW, CMake 4 y Ninja |
| [Estabilidad y crashes](estabilidad-y-crashes.md) | 2026-10-26 | 1 | Arranca y se juega; traza de pila real en `stdout.txt` |
| [Recursos gráficos](recursos-graficos.md) | 2026-10-26 | 2 | Modo remake por defecto; quedan gráficos inexistentes |
| [Audio](audio.md) | 2026-10-26 | 2 | Alias de botones a `btn.wav`; `main_menu` sin verificar |
| [Localización](localizacion.md) | 2026-10-26 | 1 | Respaldo a inglés; `caesar.sp` reparado |
| [Interfaz (GUI)](interfaz-gui.md) | 2026-10-26 | 0 | Diálogos y fuentes sin avisos |
| [Scripting JS](scripting-js.md) | 2026-10-26 | 0 | `TypeError` de menú y lobby corregidos |
| [Mundo e imperio](mundo-e-imperio.md) | 2026-10-26 | 2 | Rutas vacías de comerciantes sin resolver |
| [Ciudad y walkers](ciudad-y-walkers.md) | 2026-10-26 | 1 | `walker.model` completo; `GameLoaderC3Sav` con edificios desconocidos |
| [Repositorio y flujo](repositorio-y-flujo.md) | 2026-10-26 | 0 | Fork `vipres/caesaria-game`, rama única `main`; skills, agentes y `/endwork` `/final` en `.claude/` |

## Estado general

- **Compilar:** `cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ..` en `build/`, luego `ninja`. Herramientas instaladas con scoop (`~/scoop/apps/mingw/current/bin` en el PATH).
- **Ejecutar:** desde `bin/` (busca `resources/` relativo al directorio de trabajo). Necesita `libwinpthread-1.dll` junto al exe.
- **Log:** `bin/stdout.txt`, se sobreescribe en cada arranque. Un crash deja traza con `rva=`; se simboliza con `addr2line -e bin/caesaria.exe 0x140000000+rva` sobre la build de `build-dbg/` (RelWithDebInfo).
- **Datos del juego:** casi toda la lógica de contenido está en `bin/resources/*.model` (texto, formato JSON laxo con comentarios). Los `.c3` son las variantes para los gráficos originales de Caesar III y no se usan por defecto.
