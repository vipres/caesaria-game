# Commands — Índice

Slash commands del fork (`/<nombre>`). Adaptados del proyecto estadillos a un
repositorio de rama única sin despliegue.

| Command | Para qué sirve |
|---|---|
| `endwork.md` | Tras una unidad de trabajo: clasifica el diff por áreas, escribe el devlog, autoalimenta skills y agentes, crea los que toque. **Sin commit.** Varias veces por sesión. `--dry-run`, `--area=`. |
| `final.md` | Cierre end-to-end: endwork + validación (ninja + arranque con `log-analyzer`) + commit en `main` + checkpoint + push a `origin/main`. Un solo punto de confirmación. |

## Solapamientos

- `/endwork` vs `/final`: `endwork` documenta sin commit; `final` lo incluye y
  además valida, commitea y empuja.
- Ambos se corrigen en la misma sesión en que un paso falla (ver
  `caesaria-devlog` § Autoalimentación).
