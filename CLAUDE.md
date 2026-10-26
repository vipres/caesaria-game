# CaesarIA — fork vipres

Fork de `dalerank/caesaria-game` (remake libre de Caesar III, C++/SDL2, GPL-3.0,
upstream parado en 2016). Objetivo: que compile y funcione con herramientas
actuales en Windows x64 y mejorar el juego. Rama única `main`; `origin` es el
fork, `upstream` el original. Idioma de trabajo y documentación: español.

## Antes de tocar nada

1. Lee `docs/devlog/README.md` (estado por áreas y pendientes) y el archivo del
   área que vayas a tocar.
2. Si el área tiene skill en `.claude/skills/INDEX.md`, invócala. Para compilar
   o ejecutar: `caesaria-build-y-diagnostico`.
3. El código se escribió para 32 bits y GCC 4/5; el toolchain es MinGW GCC 16
   (scoop), CMake 4 y Ninja. Detalles y trampas en la skill de build.

## Herramientas

| Capa | Dónde | Qué hace |
|---|---|---|
| Devlog | `docs/devlog/<area>.md` | Diario por área: pendientes + historial con causa y commit |
| Skills | `.claude/skills/` (2) | Conocimiento previo por área; se crean cuando el área tiene entidad |
| Agentes | `.claude/agents/` (1) | Procedimientos con contexto aislado (`log-analyzer`) |
| Commands | `.claude/commands/` (2) | `/endwork` documenta sin commit; `/final` valida, commitea y empuja |

Las tres capas se autoalimentan en cada cierre: protocolo en
`.claude/skills/caesaria-devlog/SKILL.md`.

## Convenciones

- Commits en inglés, imperativo, con cuerpo en bullets. `git add` de rutas
  concretas, nunca `-A`.
- Archivos del repo en CRLF (`core.autocrlf=true`); los scripts que reescriben
  archivos conservan el final de línea del original.
- No importar arte derivado de Caesar III desde las nightlies; solo recursos
  propios del remake.
- Datos del juego en `bin/resources/*.model` (JSON laxo con comentarios); los
  `.c3` son variantes para gráficos originales y no se usan por defecto.
