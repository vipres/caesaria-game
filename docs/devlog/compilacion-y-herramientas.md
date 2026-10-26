# Compilación y herramientas

Toolchain, CMake, dependencias en `dep/` y todo lo necesario para pasar de código a
`bin/caesaria.exe`. El código es de 2016 (upstream parado en `a7d2f7f`) y las
dependencias van incluidas en el repo (SDL2 parcheado, freetype, curl, libpng 1.4...).

## Pendientes

- [ ] Las dependencias en `dep/` son muy antiguas (libpng 1.4.5, curl de 2012). Funcionan, pero cualquier actualización implicará rehacer los parches de compatibilidad.

## Historial

### 2026-10-26 · Build limpia con MinGW GCC 16, CMake 4.4 y Ninja
**Tipo:** fallo
**Síntoma:** ~3800 errores de compilación con GCC 16; CMake 4 rechaza `cmake_minimum_required(VERSION 2.6)`.
**Causa:** código escrito para GCC 4/5. GCC 14+ convierte en error las conversiones de puntero incompatibles, GCC 15 compila C en C23 por defecto (los prototipos vacíos de curl dejan de admitir argumentos), y `-Wnarrowing` es error en C++11.
**Cambio:** commit `9e1e64b`.
- `source/core/vector2.hpp`: usaba `other.X`/`.Y` (miembros inexistentes) en plantillas que GCC antiguo no comprobaba.
- `dep/aes/sha2.h`: `sha2_64t` debe ser `uint64_t`, no `int64_t`.
- `source/objects/house.cpp`, `source/gfx/picture_bank.cpp`: comparar punteros con `nullptr`, no con `0`.
- `dep/mixer/load_ogg.c`, `music_ogg.c`: los callbacks de tremor devuelven `int`/`long`, no `Sint64`.
- `dep/ttf/SDL_ttf.c`: `TTF_RenderUTF8_Blended_TX` devuelve `int`; el macro devolvía `NULL`.
- `dep/curl/select.h`: winsock2 de MinGW ya define `struct pollfd`.
- `CMakeLists.txt`: `-std=gnu11` para todo el C cuando el compilador es GCC o Clang.
**Verificación:** `ninja` termina sin errores para juego, `updater` y `tileset`. Configurar con `-DCMAKE_POLICY_VERSION_MINIMUM=3.5`.
**Notas:** el mensaje "has no C++11 support" del CMake del juego es un texto mal colocado; la prueba sí tiene éxito y añade `-std=c++11`. `build/` es Release y `build-dbg/` RelWithDebInfo; ambas escriben en `bin/caesaria.exe`, así que no se pueden compilar a la vez.

### 2026-10-26 · Instalación del toolchain sin permisos de administrador
**Tipo:** decisión
**Cambio:** `scoop install mingw cmake ninja`. Las carpetas de Visual Studio existentes estaban vacías (sin `cl.exe`). Se eligió MinGW porque es lo que usaba el proyecto original para sus builds de Windows.
**Notas:** el exe resultante es de 64 bits; las nightlies oficiales eran de 32 bits, lo que destapó bugs de truncamiento de punteros (ver [estabilidad](estabilidad-y-crashes.md)).
