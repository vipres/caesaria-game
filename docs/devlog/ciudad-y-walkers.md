# Ciudad y walkers

Simulación de la ciudad (`source/city/`), edificios (`source/objects/`), personajes
(`source/walker/`) y sus datos: `walker.model`, `construction.model`, `house.model`,
`layers_opts.model`, `services.model`, misiones.

## Pendientes

- [ ] Cargar mapas `.sav` originales produce `GameLoaderC3Sav: Unknown building 862..867`: ids de objetos de mapa de Caesar III sin equivalente en `loader_sav.cpp`. Hay que identificar qué son (probablemente variantes de rocas o árboles) y mapearlos.

## Historial

### 2026-10-26 · `walker.model` completo
**Tipo:** fallo
**Síntoma:** `Unknown walker info for type 1` (inmigrantes, ×9), `20` (ovejas), `7` (servicios), `46` (`waterGarbage`).
**Causa:** el modelo solo tenía 21 de los 67 tipos del enum `walker::Type` (`source/walker/constants.hpp`); `WalkerHelper::getOptions` avisa por cada tipo sin entrada.
**Cambio:** entradas generadas para todos los tipos que faltaban, con `nation : "roman"` en los ciudadanos romanos y `{ }` en animales, proyectiles y enemigos. Igual en `walker.c3`. Commit `590b19c`.
**Notas:** la numeración del enum no es obvia porque hay comentarios entre medias; contar con el archivo delante.

### 2026-10-26 · Capas, tipos y servicios con nombres incorrectos
**Tipo:** fallo
**Síntoma:** `can't find type for typeName seante/doctor/nativeCenter/nativeField`, `CityServiceFactory: not found creator for service audio_player`.
**Causa:** `layers_opts.model` usaba un typo (`seante`), un nombre de walker (`doctor`, el edificio es `clinic`) y nombres en camelCase donde el código registra `native_center`/`native_field`. `audio_player` no existe como servicio.
**Cambio:** datos corregidos. Commit `590b19c`.

### 2026-10-26 · Carros especiales y almacén vacío
**Tipo:** fallo
**Síntoma:** `Can't find type for goodName none/immigrantScarb/circusCart`, `Storage::load from empty stream` (×30).
**Causa:** `AnimationBank::loadCarts` pasaba todas las claves de `carts.model` por `good::Helper::type`, incluidas tres que no son mercancías; `Storage::load` con mapa vacío es el caso normal de un objeto nuevo.
**Cambio:** las tres claves se saltan antes de la búsqueda; el aviso del almacén baja a debug. Commit `590b19c`.

### 2026-10-26 · Rotación del mapa
Ver [estabilidad y crashes](estabilidad-y-crashes.md): guardas de puntero nulo en `Tilemap::turnLeft/turnRight`.
