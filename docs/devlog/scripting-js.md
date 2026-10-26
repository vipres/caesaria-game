# Scripting JS

Motor mujs embebido (`source/scripting/`), scripts en `bin/resources/system/*.js`.
Buena parte de la UI del lobby y del menú de construcción está en JS;
`engine.getOption` devuelve `undefined` para opciones inexistentes.

## Pendientes

(ninguno)

## Historial

### 2026-10-26 · `TypeError: cannot convert undefined to object`
**Tipo:** fallo
**Síntoma:** en `sim_ui_menu.js:199` (`OnChangeBuildingOptions`) y en `lobby.js:114` (`changePlayerName`).
**Causa:** `sim.ui.menu.extmenu` se vacía con `{}` en `clean()` y `changeBuildingOptions` accedía a `.buttons[i]` sin comprobar; `lobby.js` solo comprobaba `=== null` y `getOption` devuelve `undefined`.
**Cambio:** guardas en `changeBuildingOptions` y `enableUndo`; `if (!playerName || …)` en el lobby. Commit `590b19c`.

### 2026-10-26 · Puente C++ → JS y mensajes de diagnóstico
**Tipo:** fallo
**Síntoma:** `!!! Undefined value for js.pcall engine_js_push when find ` (tipo vacío), `JS:LoadArchive can't load file C3.sg2`, `STACK state 9`.
**Cambio:** `Variant::Invalid` se empuja como `undefined` sin aviso; si el archivo que pide `LoadArchive` no existe se registra como info (es lo normal sin recursos originales); `STACK state` baja a debug. Commit `590b19c`.

### 2026-10-26 · Nombres de imagen en scripts
**Tipo:** fallo
**Cambio:** `splash_common.js` pedía el fondo `fresca` (los archivos son `freska_0000N`); `sim_ui_topmenu.js` ponía icono `none` índice 1 cuando no hay icono, lo que pedía `none_00001`. Además `SplashScreen::setBackground` carga primero `imagen_índice` en lugar de probar el nombre sin índice. Commit `590b19c`.
