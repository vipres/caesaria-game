# Mundo e imperio

Mapa del imperio, ciudades, naciones, rutas comerciales y eventos pospuestos de
misión. Código en `source/world/` y `source/events/`; datos en `cities.model`,
`relations.model`, `empire_objects.model`, `worldmap.model`, `missions/*.mission`.

## Pendientes

- [ ] `World.MovableObject: way are empty` (×15 por partida) y `cannot find way from [1500,0] to [1020,810]`: un objeto móvil del imperio nace en `[1500,0]` (parece un valor sin inicializar) y no obtiene ruta. Buscar quién lo crea sin posición (comerciante o ejército) en `source/world/`.
- [ ] `WARNING !!! can't find type for typeName all` una vez al cargar la misión aleatoria. `Cities::load` ya ignora la clave `all`; el aviso viene de otra llamada a `object::toType` aún sin localizar.

## Historial

### 2026-10-26 · Nombres de nación y clave `all`
**Tipo:** fallo
**Síntoma:** `Can't find nation type for rome` (×4) y `for greek`; `!!! Cant find city all on load`.
**Causa:** el código registra `roman` y `graeci` (`world/nation.cpp`), pero `walker.model`, `relations.model`, `cities.model` y tres misiones usaban `rome`/`greek`. En `random.mission`, `empire.cities.all` es una opción global y `Cities::load` la trataba como ciudad.
**Cambio:** datos corregidos; `Cities::load` salta la clave `all`. Commit `590b19c`.

### 2026-10-26 · `health_updater` como evento de misión
**Tipo:** fallo
**Síntoma:** `GameEventFactory: not found creator for event health_updater` en misiones de tutorial.
**Causa:** `PostponeEvent::_exec` prueba por orden request → evento → servicio de ciudad; `health_updater` es un servicio y la fábrica de eventos avisaba antes de llegar al tercer paso.
**Cambio:** `EFactory::canCreate()` y comprobación previa en `executeEvent`. Commit `590b19c`.

### 2026-10-26 · Modelo de objetos del imperio vacío
**Tipo:** decisión
**Cambio:** `empire_objects.model` es `{}` en el repo; `Empire::_initializeObjects` lo registraba como aviso y ahora es info. Los `ObjectsFactory: cannot create empire object from type null` vistos en una sesión con `.sav` originales provienen de ese cargador, no del modelo. Commit `590b19c`.
