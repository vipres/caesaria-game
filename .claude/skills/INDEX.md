# Skills — Índice

Skills propias del fork de CaesarIA. Cargan **conocimiento** antes de tocar un
área; los agentes (`../agents/INDEX.md`) ejecutan procedimientos; los comandos
(`../commands/INDEX.md`) orquestan el cierre de sesión. El diario por áreas está
en `docs/devlog/`.

| Skill | Cuándo invocar |
|---|---|
| `caesaria-devlog` | Cubre el área repositorio-y-flujo (`.claude/`, `docs/`). Al cerrar cualquier tarea sobre el juego, antes de cada commit, al empezar a trabajar en un área (leer su histórico). Gobierna la creación y autoalimentación de las demás skills y de los agentes. |
| `caesaria-build-y-diagnostico` | Antes de compilar, lanzar el juego, leer `bin/stdout.txt`, investigar un crash o tocar `CMakeLists.txt`/`dep/`. |

## Criterio para crear una skill nueva

Definido en `caesaria-devlog` § "Skills de área": cuatro o más entradas de al
menos dos sesiones distintas en el historial del área, o dos o más pendientes que exijan conocimiento previo, o
fricción (releer devlog/código más de una vez en una sesión), o una feature de
varias sesiones. Nombre `caesaria-<area>`, plantilla en esa misma sección,
registro aquí y entrada de tipo decisión en el devlog del área.

## Candidatas evaluadas (2026-10-26)

| Área | Estado | Motivo |
|---|---|---|
| Recursos gráficos | candidata | 4 entradas y 2 pendientes, pero todas de una sesión; crear en la primera sesión que vuelva a tocar `gfx/` o los zips |
| Ciudad y walkers | candidata | 4 entradas y 1 pendiente, todas de una sesión; mismo criterio que gráficos |
| Mundo e imperio | no todavía | los dos pendientes son bugs concretos, no conocimiento recurrente |
| Datos `.model` (transversal) | candidata | el formato laxo y el mapeo nombre→enum (`EnumsHelper`, sin alias) se repite en cuatro áreas; si vuelve a aparecer, skill `caesaria-modelos-de-datos` |
