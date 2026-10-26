# Audio

Motor en `source/sound/engine.cpp`, paquetes en `bin/resources/audio/*.zip`, alias en
`sounds.model` (remake) o `sounds.c3` (gráficos originales), ambientales en
`ambientsounds.model`.

## Pendientes

- [ ] `SoundEngine: could not find sound main_menu` apareció dos veces en una sesión (`lobby.js` lo pide como tema). El archivo `main_menu.ogg` está en `themes.zip` y el zip estaba montado; no se ha reproducido en el arranque ni encontrado la causa. Los hashes de índice y búsqueda coinciden (`Path::hash` y `Hash` son la misma función).
- [ ] Los alias de los botones del menú apuntan todos al clic genérico; si se quiere variedad habría que añadir sonidos propios al pack.

## Historial

### 2026-10-26 · Sonidos del menú de construcción y ambientales
**Tipo:** fallo
**Síntoma:** `could not find sound temples`, `water`, `emptyland_00005` al jugar; `Can't find file sounds.model` al arrancar.
**Causa:** los botones de `sim_ui_menu.js` usan nombres de wav de Caesar III (`temples`, `water`, 30 en total) que no existen en los zips del remake; `emptyland_00005` no está en `wavs_buildings.zip` (sí 1-4, 6, 7); en modo remake el archivo de alias es `sounds.model`, que no existía.
**Cambio:** `sounds.model` nuevo con los 30 nombres del menú mapeados a `btn` (`btn.wav` de `wavs_game.zip`); `emptyland_00005` eliminado de `ambientsounds.model`; `audio_player` eliminado de `services.model` y `city_services.model` (no existe ningún servicio con ese nombre en el código). Commit `590b19c`.
**Verificación:** pendiente de partida del usuario tras el cambio.
**Notas:** el alias se aplica en `Engine::Impl::resetIfalias` antes de buscar el archivo; `findFullPath` prueba `.ogg` y `.wav` en el cwd y en los archivos montados.
