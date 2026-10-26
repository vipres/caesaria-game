---
name: final
description: Cierre de sesión completo del fork de CaesarIA — ejecuta endwork (devlog + skills + agentes), valida que compila y arranca sin crash ni avisos nuevos, commitea en main y, tras un único checkpoint, empuja a origin/main. No crea releases ni toca upstream.
allowed-tools: Bash, PowerShell, Read, Edit, Write, Glob, Grep, Skill, Agent
---

# Final — cerrar la sesión de trabajo

```
endwork  →  validación (build + arranque)  →  commit  →  [checkpoint]  →  push
```

Un solo punto de confirmación: antes del `git push`. Todo lo anterior es local
y reversible.

## Lo que NO hace

- No crea tags ni releases de GitHub.
- No toca `upstream` (dalerank). Solo `origin` (el fork).
- No invoca `/endwork` como slash command: lee `.claude/commands/endwork.md`
  y ejecuta sus pasos, para no acoplar comandos entre sí.

## Paso 0 — Pre-check

```bash
git branch --show-current     # main
git status --short
git worktree list
git log origin/main..HEAD --oneline
```

Reglas duras:

- Rama distinta de `main` → para y pregunta.
- Worktrees de pruebas vivos (`.claude/worktrees/*`) → avisa; no se incluyen.
- Prohibido: `git rebase`, `git reset --hard`, `git push --force`, `git add -A`
  y `git add .` (el repo tiene `bin/resources/settings.model` generado y
  carpetas `build*/` que no deben entrar aunque `.gitignore` falle).
- Conflicto en cualquier paso → para y reporta. Nunca resolver por tu cuenta.

## Fase 1 — Endwork

Lee `.claude/commands/endwork.md` y ejecuta sus Pasos 1 a 5. Salta su Paso 6:
el reporte va consolidado al final. Sin commit.

## Fase 2 — Validación

Solo si el diff toca `source/`, `dep/`, `CMakeLists.txt` o `bin/resources/`.
Si solo cambió documentación o `.claude/`, salta a la Fase 3.

1. **Compila** (sigue `caesaria-build-y-diagnostico`):

   ```bash
   ninja -C build 2>&1 | tail -20
   ```

   Error → **para**. No se commitea código que no compila.

2. **Arranca y clasifica el log**: lanza el agente `log-analyzer` (20 s). Su
   informe tiene que decir "Crash: ninguno". Avisos nuevos:
   - Si proceden del cambio de la sesión y son aceptables, añádelos a
     "Pendientes" del área antes de seguir (el devlog ya los conoce para la
     próxima vez).
   - Si indican una regresión, **para** y arréglala.

## Fase 3 — Commit

1. `git add` de rutas específicas: código, datos, `docs/devlog/`, `.claude/`,
   `CLAUDE.md`. Nunca `-A`.
2. Mensaje en inglés, imperativo, como los existentes (`Clean up startup and
   in-game log warnings`), cuerpo con bullets de qué cambió y la línea de
   coautoría que indique la sesión.
3. Las entradas nuevas del devlog citan el commit por su **título** («Add
   devlog, …»), no por hash: el hash no existe hasta commitear y cualquier
   `--amend` lo cambiaría. En el siguiente `/endwork`, sustituye los títulos de
   commits ya empujados por su hash corto (`git log --oneline -5`).

   ```bash
   git commit -F - <<'EOF'
   <Título imperativo>

   - <qué cambió y por qué, una línea por bloque>

   Co-Authored-By: <según la sesión>
   EOF
   ```

## Fase 4 — Checkpoint y push

```bash
git log origin/main..HEAD --oneline
git diff --stat origin/main..HEAD
```

Muestra y espera respuesta:

```
✓ Fase 1 endwork      — <n> entradas, skills: <lista>, agentes: <lista>
✓ Fase 2 validación   — ninja OK · arranque 20 s sin crash · avisos nuevos: <n>
✓ Fase 3 commit       — <hash> <título>

⏸  Fase 4 — Empujar a origin/main
   <n> commit(s): <lista>
   ¿Empujo? (s/N)
```

Sin `s` explícito → para. El trabajo queda commiteado en local.

Con `s`:

```bash
git push origin main
```

Rechazo por no fast-forward (alguien empujó desde otra máquina) →
`git fetch origin && git merge origin/main`; conflicto → para.

## Reporte final

```markdown
# Final — sesión cerrada

| Fase | Resultado |
|---|---|
| 1 · endwork    | <n> entradas en <áreas>; skills <actualizadas/creadas>; agentes <...> |
| 2 · validación | ninja ✓ · arranque ✓ · avisos nuevos <n> (anotados en <area>) |
| 3 · commit     | `<hash>` <título> |
| 4 · push       | origin/main = `<hash>` | no empujado |

## Pendientes abiertos tras esta sesión
<los tres o cuatro más relevantes de docs/devlog/README.md>
```

## Si algo falla a mitad

| Falló en | Estado | Cómo seguir |
|---|---|---|
| Fase 1 | devlog a medias, sin commit | corrige y relanza `/final` |
| Fase 2 | sin commit | arregla el código y relanza |
| Fase 3 | commit hecho, sin push | relanza: la Fase 1 saldrá vacía y la 4 empuja |
| Fase 4 | rechazado | `git fetch` + merge y relanza |

Si un paso de este comando resultó equivocado o insuficiente, corrígelo en
este archivo antes de cerrar (regla de autoalimentación de `caesaria-devlog`).
