---
name: endwork
description: Cierre de una unidad de trabajo SIN commit. Clasifica el diff por áreas del juego, escribe las entradas del devlog, autoalimenta las skills y agentes afectados, y crea skills o agentes nuevos si el área ya tiene entidad. Usar varias veces por sesión; /final lo incluye.
argument-hint: [--dry-run | --area=<area>]
allowed-tools: Bash, PowerShell, Read, Edit, Write, Glob, Grep, Skill
---

# Endwork — documentar y autoalimentar sin commit

Deja el repositorio listo para que la siguiente sesión (o `/final`) encuentre
todo escrito donde toca. **No hace commit**: eso es la Fase 2 de `/final`.

## Input

- Vacío → analiza la sesión actual (diff + conversación).
- `--area=<area>` → fuerza el área (nombre de archivo de `docs/devlog/` sin `.md`).
- `--dry-run` → muestra qué escribiría, sin tocar nada.

## Paso 0 — Estado

```bash
git branch --show-current          # debe ser main (rama única del fork)
git status --short --untracked-files=all   # sin -uall los directorios nuevos salen colapsados
git diff --name-only HEAD                   # no incluye archivos sin seguimiento
git worktree list                  # si hay worktrees de pruebas, avisar
```

Si la rama no es `main`, para y pregunta: este fork trabaja en una sola rama.

## Paso 1 — Clasificar el trabajo por áreas

1. Toma la lista de archivos del Paso 0 (modificados, nuevos y staged).
2. Clasifícalos con la tabla carpeta→área de
   `.claude/skills/caesaria-devlog/SKILL.md` § Estructura. Es la fuente única;
   no inventes otra aquí.
3. Añade las áreas de trabajo que **no** dejaron diff: una investigación que
   descartó una hipótesis, una limitación aceptada, una decisión. Salen de la
   conversación, no del `git diff`.
4. Si un archivo no encaja en ninguna área, es señal de área nueva: ver Paso 5.

## Paso 2 — Devlog (skill `caesaria-devlog`)

Invoca la skill `caesaria-devlog` (si no aparece en la lista de skills, lee
`.claude/skills/caesaria-devlog/SKILL.md` directamente: las skills del proyecto
solo se cargan cuando la sesión arranca dentro de `caesaria-game/`) y, para
cada área del Paso 1:

- Si la entrada de esta unidad de trabajo ya existe (escrita a mano durante la
  sesión), amplíala; no la dupliques.
- Entrada nueva al principio de "Historial" con la plantilla (tipo, síntoma,
  causa, cambio, verificación, notas). Sin commit todavía, el campo **Cambio**
  cita el título previsto del commit; el hash se pone en el siguiente cierre,
  cuando el commit ya existe y es estable (`git log --oneline -5`). Aprovecha
  este paso para sustituir títulos de entradas anteriores por su hash.
- Ajusta "Pendientes": borra lo resuelto, añade lo que quedó abierto.
- Actualiza la fila del área en `docs/devlog/README.md`.
- Verifica contra el código lo que vas a afirmar como causa. Si no cuadra,
  pendiente a confirmar, no entrada cerrada.

## Paso 3 — Autoalimentar skills existentes

Para cada área del Paso 1 que tenga skill en `.claude/skills/INDEX.md`:

1. Lee su `SKILL.md`.
2. Aplica la tabla de autoalimentación de `caesaria-devlog`: archivos nuevos →
   Archivos críticos y Triggers; regla nueva → Reglas y trampas; causa raíz o
   trampa → Aprendizajes acumulados (`### YYYY-MM-DD · título`, con origen);
   comando nuevo → Comandos.
3. Si durante la sesión la skill dio una instrucción falsa o incompleta,
   corrígela ahora y anótalo en Aprendizajes.
4. Si no hay nada generalizable, no la toques. Un detalle puntual del bug de
   hoy no es un aprendizaje.

Regla de oro: si abriste un archivo del área que **no** aparece en "Archivos
críticos" de su skill, ese archivo entra.

## Paso 4 — Crear skills o agentes nuevos si toca

Primero refresca los números de todas las áreas, no solo las tocadas, para que
la tabla "Candidatas evaluadas" de `.claude/skills/INDEX.md` no caduque:

```bash
grep -c '^### ' docs/devlog/*.md       # entradas por área
grep -c '^- \[ \]' docs/devlog/*.md    # pendientes por área
```

Después, para cada área del Paso 1 **sin** skill, comprueba el criterio de
`caesaria-devlog` § Skills de área (≥4 entradas en historial, o ≥2 pendientes
que exigen conocimiento previo, o fricción de releer, o feature multi-sesión):

- Se cumple → crea `.claude/skills/caesaria-<area>/SKILL.md` con la plantilla,
  rellenada desde el devlog y verificada contra el código. Regístrala en
  `.claude/skills/INDEX.md` y escribe en el devlog del área una entrada de tipo
  decisión "Skill creada".
- No se cumple → añade o actualiza su fila en "Candidatas evaluadas" de
  `.claude/skills/INDEX.md` con el motivo. Así la siguiente sesión no reevalúa
  desde cero.

Agentes: si en la sesión se hizo a mano un procedimiento que ya se había hecho
antes (lanzar y clasificar el log, comparar un `.model` con su enum, batería de
arranques), crea o actualiza el agente en `.claude/agents/` y su `INDEX.md`.
Si el procedimiento existente cambió (nuevos avisos conocidos, otro comando),
edita el agente ahora.

## Paso 5 — Autoalimentar la propia herramienta

- Área nueva → crea `docs/devlog/<area>.md`, fila en el README, línea en la
  lista y en la tabla de mapeo de `caesaria-devlog/SKILL.md`.
- Un paso de este comando o de `/final` falló, sobró o faltó → corrige el `.md`
  del comando en esta misma sesión. Los comandos se mantienen igual que las
  skills: si no se corrigen cuando fallan, no se corrigen nunca.
- Si cambió el inventario (skill, agente o comando nuevo) → actualiza los
  `INDEX.md` afectados y la sección "Herramientas" de `CLAUDE.md`.

## Paso 6 — Reporte (sin commit)

```markdown
# Endwork

## Áreas
- <area>: <n> entradas nuevas, <m> ampliadas, pendientes <+a/-b>

## Skills
- `<skill>`: <qué sección se actualizó> | sin cambios | CREADA (motivo)
- Candidatas: <area> — <por qué todavía no>

## Agentes
- `<agente>`: actualizado (<qué>) | CREADO | sin cambios

## Herramienta
- <devlog skill / comandos / INDEX / CLAUDE.md>: <cambio> | sin cambios

## Siguiente
- `/final` para validar, commitear y empujar, o seguir trabajando.
```

Con `--dry-run`, ejecuta todo el análisis y muestra este reporte con lo que
**haría**, sin escribir ningún archivo.
