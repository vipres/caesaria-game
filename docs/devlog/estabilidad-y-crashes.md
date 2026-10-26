# Estabilidad y crashes

Fallos que cierran el juego, manejo de excepciones y herramientas para diagnosticarlos.
Código relevante: `source/core/stacktrace.cpp`, `source/vfs/`.

## Pendientes

- [ ] `World.MovableObject: way are empty` y `cannot find way from [1500,0]`: un objeto del mapa del imperio arranca en una posición inválida y nunca encuentra ruta. No cierra el juego, pero es un bug de lógica (ver [mundo e imperio](mundo-e-imperio.md)).

## Historial

### 2026-10-26 · Crash al arrancar: handle de `_findfirst` truncado en x64
**Tipo:** fallo
**Síntoma:** "CaesarIA: Unhandled exception" justo después de `Game: load fonts`. Bajo gdb no fallaba.
**Causa:** `vfs/filesystem.cpp` guardaba el handle de `_findfirst` en un `long` (32 bits en Windows x64) y `_findnext` escribía en una dirección truncada. Bajo el depurador el handle casualmente cabía en 32 bits.
**Cambio:** `intptr_t hFile` y comparación con `(intptr_t)-1`. Commit `9e1e64b`.
**Verificación:** arranque sin depurador hasta el menú, 20 s vivo, log sin `crashed`.
**Notas:** cualquier `long` que guarde un puntero o handle es sospechoso en este código base; se escribió para 32 bits.

### 2026-10-26 · Crash al rotar el mapa
**Tipo:** fallo
**Síntoma:** cierre al pulsar rotar; traza en `gfx::Tile::setMaster` desde `Tilemap::turnRight`.
**Causa:** `turnLeft`/`turnRight` recalculan el tile maestro de cada edificio y `_d->ate()` devuelve `nullptr` cuando la posición queda fuera del mapa; el código lo desreferenciaba sin comprobar.
**Cambio:** guardas en `gfx/tilemap.cpp` (`turnLeft`, `turnRight`, `saveMasterTiles`): se salta el tile con un aviso. Commit `9e1e64b`.
**Verificación:** partida jugada por el usuario rotando el mapa sin cierre.

### 2026-10-26 · Traza de pila real en Windows
**Tipo:** feature
**Síntoma:** el manejador de excepciones imprimía `Exception: %s (0x%08x)` literal y ninguna traza.
**Causa:** la función `Stacktrace()` estaba comentada entera y las llamadas al logger usaban formato `printf` cuando el logger es estilo `fmt` (`{}`).
**Cambio:** `core/stacktrace.cpp` reescrito con `StackWalk64` (x86 y x64), símbolos por `SymGetSymFromAddr64`, impresión del `rva` de cada frame y de la dirección de la violación de acceso. Enlace con `dbghelp` en `source/CMakeLists.txt`. Commit `9e1e64b`.
**Verificación:** el crash de `_findnext` produjo una traza de 14 frames que llevó directamente a `filesystem.cpp:605`.
**Notas:** para simbolizar: `addr2line -e bin/caesaria.exe -f -C 0x140000000+rva` con la build de `build-dbg/`.
