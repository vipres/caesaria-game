# Agentes — Índice

Agentes invocables con la herramienta `Agent` (`subagent_type` = nombre del
archivo). Cada uno ejecuta un **procedimiento** con contexto aislado y devuelve
un informe corto. El conocimiento lo cargan las skills (`../skills/INDEX.md`);
un agente de área empieza leyendo la skill de su área.

| Agente | Cuándo invocar |
|---|---|
| `log-analyzer.md` | Tras compilar, tras una partida del usuario o para verificar un arreglo: lanza el juego o lee `bin/stdout.txt`, filtra los avisos ya conocidos del devlog y devuelve solo lo nuevo por área. |

## Criterio para crear un agente

Definido en `caesaria-devlog` § "Agentes": el mismo procedimiento se ha hecho a
mano dos veces y lo útil es el informe, no los pasos. Frontmatter con `name`,
`description` (qué hace y cuándo lanzarlo), `tools` y `model`. Registrar aquí.
Un agente se actualiza en el mismo cierre de sesión en que su procedimiento
cambió (lista de avisos conocidos, comandos, pasos).
