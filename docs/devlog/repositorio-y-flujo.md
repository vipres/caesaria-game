# Repositorio y flujo de trabajo

Fork, ramas, commits y convenciones del repositorio.

## Pendientes

(ninguno)

## Historial

### 2026-10-26 · Skills, agentes y comandos de cierre en `.claude/`
**Tipo:** decisión
**Cambio:** `caesaria-devlog` pasa a gobernar tres capas: devlog (`docs/devlog/`), skills de área (`.claude/skills/`, criterio de creación objetivo: ≥4 entradas, ≥2 pendientes con conocimiento previo, fricción o feature multi-sesión) y agentes (`.claude/agents/`, procedimientos repetidos dos veces). Primera skill de área `caesaria-build-y-diagnostico` y primer agente `log-analyzer`. Comandos `/endwork` (documenta y autoalimenta sin commit) y `/final` (endwork + ninja + arranque + commit + checkpoint + push), adaptados de `estadillos/.claude/commands/` a un repo de rama única sin deploy. `CLAUDE.md` con el inventario. Commit «Add devlog, area skills, log-analyzer agent and /endwork /final commands».
**Verificación:** `/endwork --dry-run` ejecutado por un subagente sobre este mismo diff; sus hallazgos (untracked colapsados en `git status`, entrada ya existente, `docs/` sin fila de mapeo, tabla de candidatas caducada, criterio de 4 entradas demasiado laxo) se aplicaron antes del commit.
**Notas:** las skills del proyecto y los comandos `/endwork` `/final` solo se cargan si Claude Code arranca con `caesaria-game/` como directorio de trabajo, no desde `Prueba juego/`. Las skills, agentes y comandos se corrigen en la misma sesión en que fallan; los `INDEX.md` de cada carpeta y la tabla "Herramientas" de `CLAUDE.md` deben reflejar el inventario real. Una entrada escrita en el mismo commit que documenta no puede citar su hash (un `--amend` lo cambia): cita el título del commit y el hash se añade en el siguiente cierre, cuando ya es estable.

### 2026-10-26 · Fork `vipres/caesaria-game` con rama única `main`
**Tipo:** decisión
**Cambio:** fork de `dalerank/caesaria-game` (GPL-3.0, upstream inactivo desde 2016). Remotos locales: `origin` = fork (SSH), `upstream` = original. Se borraron `master` y las 16 ramas heredadas; `main` es la rama por defecto y parte del último commit de upstream (`a7d2f7f`).
**Notas:** el clon es shallow; `git fetch --unshallow upstream` recupera el historial completo si hace falta. Los assets `pics_*.zip` del repo son conversiones del arte de Caesar III que upstream ya distribuía; no añadir más arte original.

### 2026-10-26 · Finales de línea
**Tipo:** decisión
**Causa:** los archivos del repo están en CRLF y `core.autocrlf=true`. Escribir con Python (`newline="\n"`) o con `sed -i` de Git Bash convierte a LF y el diff muestra el archivo entero cambiado.
**Notas:** al generar archivos con scripts, conservar el estilo del original (leer bytes, detectar `\r\n`, escribir igual). El commit `590b19c` se normalizó así antes de confirmarse.

### 2026-10-26 · Commits
- `9e1e64b` Build with modern GCC on Windows x64 and fix startup/rotation crashes.
- `590b19c` Clean up startup and in-game log warnings.
