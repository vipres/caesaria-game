---
name: caesaria-build-y-diagnostico
description: Cómo compilar, ejecutar y diagnosticar el fork de CaesarIA en Windows x64 (MinGW GCC 16 vía scoop, CMake 4, Ninja, builds Release y RelWithDebInfo, log bin/stdout.txt, trazas de pila con rva y addr2line, finales de línea CRLF). Úsala SIEMPRE antes de compilar, lanzar el juego, leer el log, investigar un crash o "Unhandled exception", tocar CMakeLists.txt o dep/, o cuando el usuario diga "compila", "arranca", "se ha cerrado", "mira el log", "no compila". No la uses para la lógica del juego (áreas de ciudad, imperio, gráficos: lee su devlog) ni para git (ver docs/devlog/repositorio-y-flujo.md).
---

# Build y diagnóstico — conocimiento vivo

Qué hay que saber antes de compilar o ejecutar `caesaria.exe`. El historial de
cómo se llegó aquí está en `docs/devlog/compilacion-y-herramientas.md` y
`docs/devlog/estabilidad-y-crashes.md`; esta skill no lo repite, lo resume
para actuar.

## Triggers explícitos

- Archivos: `CMakeLists.txt` (raíz y `source/`), `dep/**`, `source/core/stacktrace.cpp`,
  `source/vfs/**`, `build/`, `build-dbg/`, `bin/caesaria.exe`, `bin/stdout.txt`.
- Mensajes: `Unhandled exception`, `crashed`, `rva=0x`, `CMake Error`, `error:` de g++,
  `has no C++11 support`, `cannot find -l`, `libwinpthread-1.dll`.
- Conceptos: compilar, build, ninja, cmake, scoop, mingw, gdb, addr2line, traza,
  arrancar el juego, log de arranque, segundos vivo, timeout.

## Archivos críticos

| Archivo | Qué controla |
|---|---|
| `CMakeLists.txt` | Flags globales; fuerza `-std=gnu11` para C con GCC/Clang (curl y mixer no compilan en C23) |
| `source/CMakeLists.txt` | Ejecutable; enlaza `dbghelp` para la traza de pila |
| `source/core/stacktrace.cpp` | `Stacktrace()` con `StackWalk64`; imprime `rva=` por frame |
| `source/vfs/filesystem.cpp` | Listado de directorios con `_findfirst`; handle en `intptr_t` (bug x64 corregido) |
| `dep/curl/select.h`, `dep/mixer/*ogg.c`, `dep/ttf/SDL_ttf.c`, `dep/aes/sha2.h` | Parches de compatibilidad GCC 14+; cualquier actualización de `dep/` los rehace |
| `bin/resources/settings.model` | Generado, ignorado por git; conserva defaults viejos: borrar al cambiar `settings.cpp` |
| `bin/stdout.txt` | Log del juego, sobrescrito en cada arranque |

## Reglas y trampas

- `build/` (Release) y `build-dbg/` (RelWithDebInfo) escriben **el mismo**
  `bin/caesaria.exe`. No compilar las dos a la vez; para simbolizar un crash hay
  que estar seguro de cuál se ejecutó.
- El juego busca `resources/` relativo al directorio de trabajo: ejecutar
  siempre con cwd `bin/`.
- El exe es de 64 bits y el código se escribió para 32: cualquier `long` que
  guarde un puntero o handle es sospechoso (`_findfirst` ya mordió).
- Un crash que no se reproduce bajo gdb suele ser truncamiento de handle o
  memoria no inicializada; fiarse de la traza `rva=` del log, no del depurador.
- Los archivos del repo están en CRLF con `core.autocrlf=true`. Scripts que
  reescriben archivos (Python, `sed -i`) deben conservar los finales de línea o
  el diff muestra el archivo entero.
- El mensaje de CMake "has no C++11 support" es falso: la prueba tiene éxito y
  añade `-std=c++11`.
- Un cierre a los 20 segundos en una prueba automatizada es el timeout de la
  prueba, no un crash. Comprobar `stdout.txt` antes de decir "se ha cerrado".
- `-Wnarrowing` y las conversiones de puntero incompatibles son errores en
  GCC 14+; no bajar el nivel de aviso, arreglar el código.

## Comandos

Configurar y compilar (desde la raíz del repo, Git Bash o PowerShell):

```bash
mkdir -p build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ..
ninja
```

Build con símbolos (para trazas legibles): igual en `build-dbg/` con
`-DCMAKE_BUILD_TYPE=RelWithDebInfo`.

Lanzar 20 s y leer el log (PowerShell):

```powershell
Set-Location bin
$p = Start-Process -FilePath .\caesaria.exe -PassThru
Start-Sleep -Seconds 20
if (-not $p.HasExited) { Stop-Process -Id $p.Id }
Select-String -Path stdout.txt -Pattern 'WARNING|crashed|Unhandled' | Select-Object -First 60
```

Simbolizar un frame de la traza (`rva=0x...` del log, build de `build-dbg/`):

```bash
addr2line -e bin/caesaria.exe -f -C 0x140000000+<rva>
```

Toolchain: `scoop install mingw cmake ninja`; `~/scoop/apps/mingw/current/bin`
en el PATH; `libwinpthread-1.dll` copiado junto al exe.

## Aprendizajes acumulados

### 2026-10-26 · Creación de la skill
Fuente: entradas del 2026-10-26 en `compilacion-y-herramientas.md` y
`estabilidad-y-crashes.md` (commits `9e1e64b`, `590b19c`). Criterio cumplido:
el área se releyó varias veces en la misma sesión para recordar flags y rutas.

## Autoalimentación

Al cerrar una sesión que compiló, lanzó o depuró el juego (`/endwork` lo
dispara): flags o pasos de build nuevos → § Comandos; archivo nuevo en `dep/`,
`vfs/` o `core/` que afecte al arranque → § Archivos críticos; causa raíz de un
crash o un falso positivo de diagnóstico → § Aprendizajes con fecha y commit;
cualquier instrucción de aquí que resultara falsa → corregirla en el acto.
