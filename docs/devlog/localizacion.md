# Localización

`source/core/locale.cpp` y `bin/resources/locale/<archivo>.<idioma>`. El idioma se
toma de `language.model` y del sistema; el español es `sp`.

## Pendientes

- [ ] Solo `caesar` tiene versión `sp`; `names`, `tutorial`, `random` y los textos de misión existen en `en`, `ru`, `de`, `ua`. Traducirlos es trabajo de contenido.

## Historial

### 2026-10-26 · `caesar.sp` no se parseaba entero
**Tipo:** fallo
**Síntoma:** `Can't parse file locale/caesar.sp error: Wrong symbol in object name "al_first_collapse##"…`; los textos posteriores a la línea 2256 salían sin traducir.
**Causa:** comillas sin escapar dentro de un valor (`un botón "Cancelar"`).
**Cambio:** `«Cancelar»`. Commit `590b19c`.

### 2026-10-26 · Respaldo a inglés cuando falta un archivo de idioma
**Tipo:** feature
**Síntoma:** `Can't find file locale/names.sp`, `tutorial.sp`, `random.en`.
**Cambio:** `Locale::addTranslation` carga `<archivo>.en` si no existe el del idioma actual, y si tampoco existe en inglés lo registra como info y sale. Commit `590b19c`.
**Notas:** `romancelebs.model` (fechas de festivales) tenía una comilla sin cerrar y un texto libre pegado dentro del objeto; se corrigió y el texto quedó como comentario `/* */`.
