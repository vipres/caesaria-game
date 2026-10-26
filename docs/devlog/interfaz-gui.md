# Interfaz (GUI)

Widgets en `source/gui/`, definiciones en `bin/resources/gui/*.gui`, fuentes en
`source/font/` y `bin/resources/fonts/`.

## Pendientes

(ninguno)

## Historial

### 2026-10-26 · Avisos de widgets opcionales, fuentes duplicadas y ficheros de GUI
**Tipo:** fallo
**Síntoma:** `Cannot find lbTitle/btnActionYes/btnActionNo in dialogbox.cpp`, `font already exists, name=FreeSerif`, `Parent for widget is null`, `Can't find file gui/minimap.gui`, `commands.model`.
**Causa:**
- `Dialogbox` busca con `INIT_WIDGET_FROM_UI` widgets que solo existen en `dialogbox_yesno.gui`, no en `dialogbox.gui`; el macro avisa siempre que no los encuentra.
- `FontCollection::getFont` registraba la fuente con el nombre de familia en vez del nombre completo (`FreeSerif_16`), así que la segunda petición con otro tamaño chocaba.
- El widget raíz de `Ui` no tiene padre por diseño.
- `minimap.gui` y `commands.model` (histórico de la consola) no existían en el repo.
**Cambio:** búsqueda silenciosa con `findChildA` para los tres widgets opcionales; `_addFont(name, …)`; el aviso del padre nulo baja a debug; `gui/minimap.gui` y `commands.model` creados vacíos. Commit `590b19c`.
