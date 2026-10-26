---
name: caesaria-devlog
description: Diario técnico del fork de CaesarIA en docs/devlog/, organizado por área del juego (compilación, crashes, gráficos, audio, localización, GUI, scripting JS, mundo/imperio, ciudad/walkers, repositorio), y gobierno de las skills y agentes de área en .claude/ que crecen a partir de él. Úsalo siempre que se cierre una tarea sobre caesaria-game (bug corregido, feature, investigación, decisión, limitación encontrada), antes de cada commit, al ejecutar /endwork o /final, y cuando el usuario diga "documenta", "apunta", "deja constancia", "devlog", "qué hemos hecho en X", "crea una skill para X" o empiece a trabajar en un área (hay que leer su histórico y cargar su skill primero). Aplica aunque el usuario no pida documentar explícitamente: si se ha tocado el juego, hay una entrada que escribir y, si el área ya pesa, una skill que crear o actualizar.
---

# Devlog de CaesarIA

El fork vive mucho tiempo y pasa por muchas sesiones. Lo que no queda escrito se
reinvestiga: cuál era la causa de un crash, por qué un aviso del log no se puede
quitar, qué archivo `.model` controla qué. Este diario existe para que cualquier
sesión futura (tuya o de otra persona) pueda retomar un área sin repetir trabajo.

Todo va en `docs/devlog/` dentro del repositorio, en español, y se versiona con
el código: la documentación de un cambio viaja en el mismo commit que el cambio.

El diario es la capa base. Cuando un área acumula conocimiento que hace falta
*antes* de tocar el código (no solo después), ese conocimiento sube a una skill
de área en `.claude/skills/`, y si además hay un procedimiento repetitivo que
conviene ejecutar con contexto aislado, a un agente en `.claude/agents/`. Las
tres capas se mantienen desde aquí (ver "Skills de área" y "Agentes").

## Cuándo escribir

- Al terminar una unidad de trabajo: un fallo corregido, una feature, una
  investigación aunque no haya producido cambios, una decisión de diseño, o una
  limitación que se decide no resolver (por ejemplo, arte que no existe).
- Antes de hacer `git commit`: la entrada se incluye en ese commit.
- Cuando el usuario lo pida con cualquier fórmula ("apunta", "documenta",
  "deja constancia") o lance `/endwork` o `/final`.

## Cuándo leer

Antes de empezar a trabajar en un área, abre su archivo y la sección
"Pendientes". Ahí están las causas ya descubiertas, lo descartado y lo que
sigue abierto. Evita, por ejemplo, volver a buscar por qué faltan ciertos
gráficos cuando ya está anotado que no existen en los paquetes del remake.
Si el área tiene skill (`.claude/skills/INDEX.md`), invócala también: la skill
lleva lo que hay que saber *antes* de editar; el devlog, lo que pasó.

## Estructura

```
docs/devlog/
├── README.md                    índice: áreas, último cambio y pendientes
├── compilacion-y-herramientas.md
├── estabilidad-y-crashes.md
├── recursos-graficos.md
├── audio.md
├── localizacion.md
├── interfaz-gui.md
├── scripting-js.md
├── mundo-e-imperio.md
├── ciudad-y-walkers.md
└── repositorio-y-flujo.md
```

Elige el área por el subsistema afectado, no por el síntoma. Un aviso del log
sobre un sonido va a `audio.md` aunque se haya visto "arreglando el log". Si un
cambio toca dos áreas, escribe la entrada completa en la principal y una línea
con enlace en la otra. Si ninguna encaja, crea un archivo nuevo con el mismo
formato, añádelo al índice y a la lista y la tabla de este SKILL.md; no fuerces
un área equivocada.

Mapa orientativo de carpetas del código a áreas (fuente única; `/endwork` lo
usa para clasificar el diff):

| Código / datos | Área |
|---|---|
| `CMakeLists.txt`, `dep/`, toolchain, flags | compilacion-y-herramientas |
| `core/stacktrace.cpp`, `vfs/`, crashes, x64 | estabilidad-y-crashes |
| `gfx/`, `bin/resources/gfx/*.zip`, `construction.model`, atlas | recursos-graficos |
| `sound/`, `bin/resources/audio/`, `sounds.model`, `ambientsounds.model` | audio |
| `core/locale.cpp`, `bin/resources/locale/` | localizacion |
| `gui/`, `bin/resources/gui/*.gui`, fuentes | interfaz-gui |
| `scripting/`, `bin/resources/system/*.js` | scripting-js |
| `world/`, `empire_objects.model`, `cities.model`, `relations.model` | mundo-e-imperio |
| `city/`, `objects/`, `walker/`, `walker.model`, misiones | ciudad-y-walkers |
| git, fork, ramas, commits, CI, `.claude/`, `CLAUDE.md`, `docs/` | repositorio-y-flujo |

## Formato de cada archivo de área

```markdown
# <Área>

Una o dos frases sobre qué cubre y dónde está el código.

## Pendientes

- [ ] Qué falta, por qué está abierto y qué haría falta para cerrarlo.

## Historial

### 2026-10-26 · Título corto que diga qué pasó
**Tipo:** fallo | feature | investigación | decisión | limitación
**Síntoma:** qué se veía (mensaje de log, crash, comportamiento).
**Causa:** la raíz, no la pista. Si no se encontró, dilo.
**Cambio:** archivos tocados y commit (`hash corto`). Si no hubo cambio, "ninguno".
**Verificación:** cómo se comprobó (comando, log, partida jugada).
**Notas:** lo que conviene saber la próxima vez (trampas, alternativas descartadas).
```

Las entradas nuevas van al principio del historial, para que lo reciente esté a
la vista. Omite los campos que no aporten nada en lugar de rellenarlos con
"n/a". Una entrada por unidad lógica: si una sesión arregló cinco cosas en un
área, son cinco entradas cortas, no una larga.

Cuando un pendiente se resuelve, bórralo de "Pendientes" y escribe la entrada en
el historial; no dejes el mismo asunto en los dos sitios.

## El índice (README.md)

Tabla con una fila por área: nombre enlazado, fecha del último cambio, número
de pendientes y una frase de estado. Debajo, una lista corta "Estado general"
con las tres o cuatro cosas que alguien debe saber antes de tocar nada (cómo se
compila, cómo se ejecuta, dónde está el log). Mantén el índice al día cada vez
que escribas una entrada; es lo primero que se lee.

## Qué escribir y qué no

- Escribe la causa y el razonamiento. El mensaje del commit ya lista archivos;
  el devlog explica por qué, qué se descartó y qué se aprendió.
- Anota las limitaciones aceptadas con su motivo (p. ej. "los frames 16, 24…
  de `carttimber` no existen en los PNG; haría falta dibujarlos"). Es la
  información que más tiempo ahorra.
- Guarda comandos exactos cuando sirvan para repetir una verificación (configurar
  CMake, simbolizar una traza con `addr2line`).
- No pegues logs enteros ni diffs: un extracto de dos o tres líneas basta.
- No narres el proceso ("primero miré… luego probé…"); escribe el resultado.
- Contrasta lo que te cuentan con el código antes de escribirlo. Si el archivo o
  la condición descritos no aparecen en el repo, díselo al usuario en una línea y
  anota la discrepancia como pendiente en vez de dar la entrada por buena: una
  causa mal apuntada cuesta más que una entrada sin escribir.
- Sé breve en "Notas": dos o tres frases con lo que ahorra tiempo la próxima vez,
  no un informe de todo lo que se miró.
- Nombres de archivo y funciones en código inline; rutas relativas al repo.

## Skills de área (cuándo el devlog ya no basta)

El devlog cuenta lo que pasó. Una skill carga lo que hay que saber antes de
tocar un área: archivos críticos, reglas que no se deducen del código, trampas,
comandos. Se crea una skill cuando la cuestión tiene entidad, y la señal es
objetiva: se cumple **cualquiera** de estas tres.

1. El área acumula **cuatro o más entradas de al menos dos sesiones distintas**
   (fechas distintas) en su historial, o **dos o más pendientes** abiertos que
   exigen conocimiento previo para abordarse. Una sola sesión productiva llena
   cuatro entradas sin que el área tenga todavía reglas que merezcan skill.
2. En una sesión hubo que **releer el devlog o el código más de una vez** para
   recordar cómo funciona algo (señal de fricción: toolchain, formato de un
   `.model`, puente C++/JS).
3. El usuario pide trabajar en una feature nueva de alcance claro (una mecánica,
   un editor, un sistema de mods) que va a durar varias sesiones.

Si no se cumple ninguna, no crees la skill: una skill vacía o prematura cuesta
contexto en cada sesión y no devuelve nada. Anota en el reporte de `/endwork`
que el área es candidata y por qué todavía no.

Cuando sí toca, el procedimiento es:

1. Nombre `caesaria-<area-o-feature>` en `.claude/skills/<nombre>/SKILL.md`.
   Usa la plantilla de abajo. Si el skill-creator (`/anthropic-skills:skill-creator`)
   está disponible y el usuario quiere probarla con evals, úsalo; si no, la
   plantilla basta.
2. La descripción del frontmatter debe ser empujona: qué hace **y** cuándo
   dispararla (archivos, mensajes de log, palabras del usuario), porque Claude
   tiende a no activar skills. Incluye también cuándo **no** usarla.
3. Rellena desde el devlog, no desde memoria: cada regla o trampa que entra en
   la skill debe existir ya en una entrada del historial o verificarse en el
   código en ese momento.
4. Regístrala en `.claude/skills/INDEX.md` (tabla "cuándo invocar") y anota en
   el devlog del área una entrada de tipo decisión: "Skill `<nombre>` creada".
5. Si la skill es grande (>300 líneas), divide en `SKILL.md` (barrel con
   triggers, reglas y punteros) y `knowledge/<tema>.md` cargados a demanda.

Plantilla mínima de una skill de área:

```markdown
---
name: caesaria-<area>
description: <qué carga> para el fork de CaesarIA. Úsala SIEMPRE antes de tocar <archivos/carpetas>, cuando el log muestre <mensajes>, o cuando el usuario hable de <conceptos>. No la uses para <área vecina> (usa <otra skill>).
---

# <Área> — conocimiento vivo

## Triggers explícitos
Archivos, mensajes de log y conceptos que deben hacer invocar esta skill.

## Archivos críticos
Por capa: código C++, datos `.model`/`.c3`, scripts JS, recursos. Una línea por archivo con qué controla.

## Reglas y trampas
Lo que no se deduce leyendo el código: convenciones del enum, formato laxo de los `.model`, finales de línea, builds que comparten `bin/`...

## Comandos
Los exactos, listos para pegar.

## Aprendizajes acumulados
### YYYY-MM-DD · <título>
Qué se aprendió, por qué importa, commit o entrada del devlog de origen.

## Autoalimentación
Qué actualizar al cerrar una sesión que tocó esta área (ver protocolo en caesaria-devlog).
```

## Agentes

Un agente (`.claude/agents/<nombre>.md`, frontmatter `name`, `description`,
`tools`, `model`) encapsula un **procedimiento** repetitivo que conviene
ejecutar fuera del contexto principal: lanzar el juego y clasificar el log,
compilar las dos builds y pasar una batería de arranques, comparar un `.model`
con el enum que lo consume. Se crea cuando el mismo procedimiento se ha hecho a
mano dos veces y el resultado útil es un informe corto, no los pasos. La skill
carga conocimiento; el agente ejecuta. Un agente de área debe empezar por leer
la skill de esa área si existe. Regístralo en `.claude/agents/INDEX.md`.

## Autoalimentación de skills, agentes y comandos

Las skills y los agentes caducan en silencio: nadie avisa de que un archivo
nuevo no aparece en "Archivos críticos" hasta que la siguiente sesión pregunta
"¿estás al día?" y la respuesta es no. Por eso cada cierre de trabajo (`/endwork`,
`/final`, o este skill a secas) revisa, para cada área tocada que tenga skill:

| Lo que pasó en la sesión | Qué actualizar en la skill |
|---|---|
| Archivo, `.model`, script o carpeta nuevos en el área | § Archivos críticos y § Triggers |
| Mensaje de log nuevo que lleva a esta área | § Triggers |
| Regla o decisión que cambia cómo se edita el área | § Reglas y trampas |
| Edge case, causa raíz no obvia, alternativa descartada | § Aprendizajes acumulados (entrada con fecha y commit) |
| Comando nuevo o corregido | § Comandos |
| La skill dio una instrucción que resultó falsa | corrige la línea en el acto y anótalo en Aprendizajes |

Lo mismo para los agentes: si el procedimiento cambió (nuevo paso, nueva lista
de avisos conocidos, otro comando), se edita el agente en ese cierre. Y para los
comandos `/endwork` y `/final`: si un paso falló o sobró al ejecutarlo, se
corrige el `.md` del comando en esa misma sesión, no "más adelante".

Este propio skill se actualiza cuando aparece un área nueva (lista, tabla de
mapeo, README del devlog) o cuando el criterio de creación de skills demostró
ser demasiado laxo o estricto.

No dupliques: lo que ya está en el devlog se enlaza desde la skill, no se copia;
lo compartido entre áreas (por ejemplo, el formato de los `.model`) vive en una
sola skill y las demás apuntan a ella.

## Procedimiento

1. Decide el área (o áreas) del trabajo terminado con la tabla de mapeo.
2. Lee el archivo del área. Si no existe, créalo con el formato de arriba y
   actualiza este SKILL.md.
3. Escribe la entrada al principio de "Historial" y ajusta "Pendientes".
4. Actualiza la fila del área en `docs/devlog/README.md`.
5. Si el área tiene skill, aplica la tabla de autoalimentación. Si no la tiene,
   comprueba el criterio de creación y créala si se cumple.
6. Si se repitió un procedimiento a mano, valora crear o actualizar un agente.
7. Si el trabajo va a un commit, incluye `docs/devlog/` y `.claude/` en ese
   mismo commit.
8. Al responder al usuario, di en una línea qué entrada has escrito y dónde, y
   qué skills o agentes se crearon o actualizaron.
