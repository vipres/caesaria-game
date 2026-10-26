# Recursos gráficos

Paquetes PNG en `bin/resources/gfx/*.zip` (cada uno con atlas `.atlas` + `info`),
`PictureBank`, `ResourceLoader` y los modelos que referencian imágenes
(`construction.model`, `carts.model`, `house.model`, `animations.model`).

## Pendientes

- [ ] Gráficos que no existen en los paquetes del remake y producen `PictureBank: Unknown resource`: `carttimber` índices 16, 24, 32… (dirección noroeste), `housng1b` 40-44, `housng1d` 27, `entertainment` 108-109 (plaza 2x2), `land1a` 18-29 y `land3a` 19-42 (costas del generador de mapas aleatorios), `sprites` 0-2 (banderas de fuertes), `taxcolumn` 0. Existen en la nightly oficial, pero son conversiones del arte original de Caesar III; decidido no importarlos al repo.
- [ ] `GameLoaderC3Sav: Unknown building 862..867` al cargar mapas `.sav` originales: ids de objetos de mapa que el cargador no conoce.

## Historial

### 2026-10-26 · Modo de gráficos del remake por defecto
**Tipo:** fallo
**Síntoma:** cientos de `Unknown resource` (`carts_00001..136`, `citizen05_00665`, `land2a_00188`…) y tipos desconocidos (`riverWave`, `doctor`) al arrancar.
**Causa:** `settings.cpp` fijaba `oldgfx = 1` por defecto (solo la build de Steam lo ponía a `false`). Con eso `checkC3present()` carga los modelos `.c3`, pensados para los gráficos originales de Caesar III, que piden imágenes que los PNG del remake no tienen.
**Cambio:** `oldgfx = false` por defecto; `isC3mode()` sigue activando `.c3` si se configura `c3gfx`. `logoArchive` ahora depende del modo (`pics_wait.zip` en C3, `pics.zip` en remake). Commit `590b19c`.
**Verificación:** log de arranque de 297 a 38 líneas.
**Notas:** el `settings.model` generado en `bin/resources/` (ignorado por git) conserva valores viejos; borrarlo tras cambiar defaults.

### 2026-10-26 · Atlas con nombre invertido en `pics_house.zip`
**Tipo:** fallo
**Síntoma:** `Can't open file bin/housng1a1.atlas` y fachadas `housng1a` pares sin imagen.
**Causa:** el `info` del zip lista `housng1a1.atlas` pero la entrada se llamaba `housgn1a1.atlas`.
**Cambio:** zip regenerado con la entrada renombrada (el PNG conserva su nombre). Commit `590b19c`.

### 2026-10-26 · Pack `pics_oc3.zip` añadido
**Tipo:** decisión
**Causa:** `archives.model` lo referencia (`oc3addon`) y `lobby.js`, `g_config_movie.js` usan `ckit`, `steam_icon`, `city_fire`, pero el repo no lo traía. Es arte propio del remake (logo del equipo, iconos), por eso se copió desde la nightly 3421. Commit `590b19c`.

### 2026-10-26 · Pequeños arreglos de datos y cargador
**Tipo:** fallo
**Cambio:** commit `590b19c`.
- `construction.model`: clases `terrain`/`rock`/`tree` → nuevo grupo `natural` (`object::group::natural`), entradas mínimas para `river` y `waymark` (tipos 91 y 93 sin metadatos), eliminada la variante `plaza_03139` sin imágenes.
- `carts.model`: el carro "mega" de trigo apuntaba a `cartwheatm`, inexistente; usa `cartwheatb`.
- `ResourceLoader::loadFiles` solo carga `.png`/`.bmp` (el archivo del logo traía `.atlas` e `info`), y avisa con la ruta original cuando un archivo no existe en vez de con cadena vacía.
- `PictureBank`: un nombre vacío devuelve imagen inválida sin aviso.
- `loader_png.cpp`: el aviso `iCCP: known incorrect sRGB profile` baja a debug (unos 250 PNG de `pics_south.zip` lo tienen; libpng 1.4 no permite desactivar la comprobación).
**Notas:** `EnumsHelper::append` guarda un solo nombre por valor, así que no se pueden registrar alias de grupo; hay que cambiar los datos.
