# Taller 7

## Agenda

1. Preliminares
2. Servicios
3. Semáforos

## Preliminares

Abra una terminal. Abra el directorio de repositorio y de los talleres

```
cd <repositorio-enlace>
cd talleres
```

Descargue el taller en formato zip, descomprimalo.

```
wget https://github.com/jfcmacro/TallerSO_07/archive/refs/heads/master.zip
unzip master.zip
rm master.zip
```

Mire la estructura actual

```
tree .
```

Entre al directorio del taller

```
cd TallerSO_07-master
```

Adicione los ficheros

```bash
git add README.md .gitignore
```

Adicione los ficheros del proyecto, esto adiciona todos ficheros del taller.

```bash
find . -name *.c -exec git add {} \;
find . -name *.h -exec git add {} \;
find . -name .keep -exec git add {} \;
find . -name makefile -exec git add {} \;
find . -name *.mk -exec git add {} \;
```

Acometa (*commit*) el proyecto.

```
git commit -m "Iniciando el Taller 07"
git push
```

## Servicios

### Servicios en Linux

El registro se Linux se hace a través del `systemd`.  `systemd` es un sistema y administrador de servicios para el sistema operativo Linux. Son varias las unidades que controla el `systemd`, desde relojes, directorios especiales como: `/sys/`, `/proc/` y `/dev/`, hasta servicios de usuarios.

En este taller vamos a instalar un servicio de usuario y a controlarlo a través del `systemd`.

#### Archivo de configuración `.service`

Es un fichero de configuración de [tipo ini](https://en.wikipedia.org/wiki/INI_file). La descripción completa de las secciones y de las claves se encuentra en [`man systemd.service(5)`](https://man7.org/linux/man-pages/man5/systemd.service.5.html).

El fichero [`systemd-service.service`](./servicios/linux/systemd-service.service) contiene tres secciones `[Unit]`, `[Service]` e `[Install]`. En la primera se describe la unidad y su propósito. Las segunda, describe el servicio en si, con el tipo, ejecutable y forma de ejecutar el programa, hasta el directorio de trabajo donde estará ubicado. La tercera sección, describe la forma que será instalado en el sistema.

#### El servicio

El fichero [`systemd-service.c`](./servicios/linux/systemd-service.c) contiene un proceso demonio, este será ejecutado como un proceso de fondo (*background process*). Para ellos es importante manejar las señales, y el registro del sistema.

#### Referencias

* [`systemd`(1)](https://man7.org/linux/man-pages/man1/systemd.1.html)
* [`systemd.service`(5)](https://man7.org/linux/man-pages/man5/systemd.service.5.html)
* [`systemd.exec`(5)](https://man7.org/linux/man-pages/man5/systemd.exec.5.html)
* [`systemd.unit`(5)](https://man7.org/linux/man-pages/man5/systemd.unit.5.html)
* [`systemctl(1)`](https://man7.org/linux/man-pages/man1/systemctl.1.html)

#### Llevar a cabo

Abrir una terminal e ir al directorio donde esta guardado el taller:

```bash 
cd servicio/linux
makefile
```
Una vez compilado, se debe instalar.

```bash
sudo make install
```

> Explicación. Profesor `systemd` , `systemctl`.

Revisar el estado del servidor a través del comando `systemctl`.

```bash
systemctl status systemd-service
```

Iniciar el servicio.

```bash
sudo systemctl start systemd-service
```

Mirar el estado actual

```bash
systemctl status systemd-service
```

Dividir la pantalla con tmux, ejecute en su pantalla:

```bash
journalctl -t systemd-service
```

Si lo quiere mantener abierto.

```bash
journalctl -t systemd-service -f
```

**[Ejercicio 1].** (Carpeta: `./servicios/linux/` Nombre: `servicio1.c`, `servicio1.service`, `cliente1.c`)

En primer lugar copie el servidor original (`systemd-service.c`), y el fichero de configuración

```bash
# En la carpeta ./servicios/linux/
cp systemd-service.c servicio1.c
cp systemd-service.service servicio1.service
```

* Formato: `servicio1`

* Formato: `cliente1`

* Descripción: El `servidor1`  crea dos tuberías (`/tmp/eco_entrada`  y `/tmp/eco_salida`), al iniciar el servicio. El `servidor1` escucha los mensajes de entrada y los translada de mayúsculas a minúsculas y viceversa. El cliente recibe mensajes de al entrada estadar, los envía a `servidor1` por la tubería `/tmp/eco_entrada` y recibe los mensajes transformados en el `/tmp/eco_salida`, y los imprime en la salida estándar. Cada mensaje recibido por `servidor1`  es puesto en el registro del sistema, mostrando el mensaje recibido y el mensaje enviado.

* Instale el servidor.

* **Nota:** No olvide modificar el fichero `makefile`

**[Ejercicio 2].** (Carpeta: `./servicios/linux/` Nombre: `servidor2.c`)

En primer lugar copie el servidor original (`systemd-service.c`), y el fichero de configuración

```bash
# En la carpeta ./servicios/linux/
cp systemd-service.c servicio2.c
cp systemd-service.service servicio2.service
```

* Formato: `servicio2`

* Descripción: El programa de control `systemctl` acepta  varias señales. El servicio debe aceptar las señales: `SIGTERM`, `SIGINT`, `SIGSTOP`. En cada señal el servicio informa la recepción de cada señal en el registro del sistema. Cuando las señales capturas alcance un contador de 5, el programa termina. Revise los manuales de configuración `systemd.service(5)`, y `systemd.exec(5)` para hacer que el servicio se reinicie automáticamente.
* **Nota**: No olvidar modificar el fichero `makefile`

### Servicios en Windows

Un servicio en Windows es un proceso de consola que se ejecuta en su propia sesion, independiente de estar conectado al sistema de sesion actual conocido como `WinLogon`.

Para crear dichos servicios, no se puede hacer como en Linux donde un proceso, se puede desconectar de su sesión y permanecer como un proceso demonio. Anteriormente esto ese podía hacer pero a partir de la versión de Windows Vista esto se cambio debido a problemas de seguridad.

Los servicios están desconectados de las sesiones no están asociados a una ventana o a una consola, por lo tanto no se puede obtener, ni generar salida. Esto requiere que para comunicarse se debe utilizar otros medios.

Para controlar el servicio utilizaremos a SCM (Service Control Management). Este permite registrar, desregistrar, iniciar, terminar, suspender o reasumir un servicio. Este es un programa que esta basado en RPC (Remote Procedure Control) que permite acceder realizar todas esas tareas. Logicamente, se debe crear el servicio primero y después realizar su registro, un después si el usuario así lo quiere desregistrarlo.

#### Servicio

Un [servicio](./servicios/windows/svrwindows.c) no es un programa de consola normal. Este tiene un punto de entrada `main`, que es llamado para registrar una tabla despacho, esta tabla contiene la información de los servicios que pueden ser manejados. El SCM permite tener más de un servicio corriendo al mismo tiempo dentro del espacio de un servicio.

En nuestro ejemplo tendremos solo un servicio, y la tabla señala cual es la función que controla el servicio (`servicio_ppal`), este puede ejecutar y controlar el servicio, en nuestro caso vamos a lanzar un hilo para que controle el servicio.

Pero tambien requiere que se registre una función que será llamada por el SCM, para indicar que ha recido las diferentes operaciones sobre el
servicio: `START`, `STOP`, `PAUSE`, `CONTINUE`, y `SHUTDOWN`.

#### Registrar un servicio

El programa [`registrar_servicio`](./servicios/windows/registrar_servicio.c), permite registrar un servicio programaticamente en el register, este programa abre primero una conexión al administrador SCM, y luego crea el servicio.

Normalmente un usuario final no lo puede hacer, solo aquellos usarios que este registrados como administradores lo pueden hacer. Para ello se debe abrir una consola en modo administrador y ejecutar el programa que registrara el servicio.

#### Desregistrar un servicio

El programa [`desregistrar_servicio`](./desregistrar_servicio.c), permite borrar el servicio previamente instalado en el register.

#### Referencias

* [About Services](https://learn.microsoft.com/es-es/windows/win32/services/about-services)

* [`OpenSCManagerA`](https://learn.microsoft.com/es-es/windows/win32/api/winsvc/nf-winsvc-openscmanagera)

* [`CreateServiceA`](https://learn.microsoft.com/es-es/windows/win32/api/winsvc/nf-winsvc-createservicea)

* [`StartServiceCtrlDispatcherA`](https://learn.microsoft.com/es-es/windows/win32/api/winsvc/nf-winsvc-startservicectrldispatchera)

* [`OpenServiceA`](https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-openservicea)

* [`DeleteService`](https://learn.microsoft.com/es-es/windows/win32/api/winsvc/nf-winsvc-deleteservice)

* [`sc create`](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/sc-create)

* [`sc_config`](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/sc-config)

* [`sc query`](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/sc-query)

* [`sc delete`](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/sc-query)

> Explicación del docente

## Semáforos

Un semáforo es un contador que una vez que ha sido inicializado permite cambiar sus valores a través dos funciones: `P()` (*Probeer*) y `V()` (*Verhogen*). En este taller vamos a ver los dos tipos de semáforos que se encuentran mayormente dentro de los sistemas operativos y algunos lenguajes de programación:

* Semáforos anónimos.
* Semáforos nombrados.

### Semáforos anónimos (No nombrados)

#### Jardin Ornamental

> Explicación docente

##### Linux

* [`jardin_ornamental.h`](./semafores/linux/jardin_ornamental.h)
* [`jardin_ornamental.c`](./semaforos/linux/jardin_ornamental.c)
* [`jardin_utilidades.c`](./semaforos/linux/jardin_utilidades.c)

> Explicacion docente implementación semáforos no nombrados en linux

###### Referencias

* [`sem_init`(3)](https://man7.org/linux/man-pages/man3/sem_init.3.html)
* [`sem_post`(3)](https://man7.org/linux/man-pages/man3/sem_post.3.html)
* [`sem_wait`(3)](https://man7.org/linux/man-pages/man3/sem_wait.3.html)
* [`sem_destroy`(3)](https://man7.org/linux/man-pages/man3/sem_destroy.3.html)
* [`sem_overview`(7)](https://man7.org/linux/man-pages/man7/sem_overview.7.html)

##### Windows

* [`jardin_ornamental.h`](./semaforos/windows/jardin_ornamental.h)
* [`jardin_ornamental.c`](./semafoso/windows/jardin_ornamental.c)
* [`jardin_utilidades.c`](./semaforos/windows/jardin_utilidades.c)

> Explicación docente implementación semáforos no nombrados en windows

###### Referencias

* [`CreateSemaphoreA`](https://man7.org/linux/man-pages/man7/sem_overview.7.html)
* [`WaitForSingleObject`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject)
* [`ReleaseSemaphore`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-releasesemaphore)
* [`CloseHandle`](https://learn.microsoft.com/es-es/windows/win32/api/handleapi/nf-handleapi-closehandle)

#### Jardin Ornamental Limitado

> Explicación docente

##### Linux

* [`jardin_ornamental_limitado.h`](./semafores/linux/jardin_ornamental_limitado.h)
* [`jardin_ornamental_limitado.c`](./semaforos/linux/jardin_ornamental_limitado.c)
* [`jardin_limitado_utilidades.c`](./semaforos/linux/jardin_limitado_utilidades.c)

> Explicacion docente implementación semáforos no nombrados en linux

###### Referencias

* [`sem_init`(3)](https://man7.org/linux/man-pages/man3/sem_init.3.html)
* [`sem_post`(3)](https://man7.org/linux/man-pages/man3/sem_post.3.html)
* [`sem_wait`(3)](https://man7.org/linux/man-pages/man3/sem_wait.3.html)
* [`sem_destroy`(3)](https://man7.org/linux/man-pages/man3/sem_destroy.3.html)
* [`sem_overview`(7)](https://man7.org/linux/man-pages/man7/sem_overview.7.html)

##### Windows

* [`jardin_ornamental_limitado.h`](./semaforos/windows/jardin_ornamental_limitado.h)
* [`jardin_ornamental_limitado.c`](./semafoso/windows/jardin_ornamental_limitado.c)
* [`jardin_limitado_utilidades.c`](./semaforos/windows/jardin_limitado_utilidades.c)

> Explicación docente implementación semáforos no nombrados en windows

###### Referencias

* [`CreateSemaphoreA`](https://man7.org/linux/man-pages/man7/sem_overview.7.html)
* [`WaitForSingleObject`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject)
* [`ReleaseSemaphore`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-releasesemaphore)
* [`CloseHandle`](https://learn.microsoft.com/es-es/windows/win32/api/handleapi/nf-handleapi-closehandle)

**[Ejercicio 3].** (Carpeta: `./semaforos/linux/` Nombre: `jardin_ornamental_general.c`, `jardin_ornamental_general.h`, `jardin_ornamental_utilidades_general.c`)

* Formato: `jardin_ornamental_general [-l <limite>] [-m <maximo>]`
* Formato: `jardin_ornamental_general [-m maximo]`

* Descripción: Implementar una versión de general del jardín, en el primer formato es la manera con limite donde $0 < limite < maximo$. La segunda es el jardin ornamental sin limite.
* **Nota**: No olvidar modificar el fichero `makefile` que incluya la creación de todos los procesos.

**[Ejercicio 4].** (Carpeta: `./semaforos/windows/` Nombre: `jardin_ornamental_general.c`, `jardin_ornamental_general.h`, `jardin_ornamental_utilidades_general.c`)

* Formato: `jardin_ornamental_general [-l <limite>] [-m <maximo>]`
* Formato: `jardin_ornamental_general [-m maximo]`

* Descripción: Implementar una versión de general del jardín, en el primer formato es la manera con limite donde $0 < limite < maximo$. La segunda es el jardin ornamental sin limite.
* **Nota**: No olvidar modificar el fichero `makefile` que incluya la creación de todos los procesos.

### Semáforos nombrados

#### Problema de los fumadores

> Explicación del docente

##### Linux

* [`fumadores.h`](./semaforos/linux/fumadores.h)
* [`agente.c`](./semaforos/linux/agente.c)
* [`fumador.c`](./semaforo/linux/fumador.c)
* [`fumadores_utilidad.c`](./semaforos/linux/fumadores_utilidade.c)
* [`ctrl_fumadores.c`](./semafores/linux/ctrl_fumadores.c)

###### Referencias

* [`sem_open`(3)](https://man7.org/linux/man-pages/man3/sem_open.3.html)
* [`sem_close`(3)](https://man7.org/linux/man-pages/man3/sem_close.3.html)
* [`sem_unlink`(3)](https://man7.org/linux/man-pages/man3/sem_unlink.3.html)
* [`sem_post`(3)](https://man7.org/linux/man-pages/man3/sem_post.3.html)
* [`sem_wait`(3)](https://man7.org/linux/man-pages/man3/sem_wait.3.html)
* [`sem_overview`(7)](https://man7.org/linux/man-pages/man7/sem_overview.7.html)

**[Ejercicio 5].** (Carpeta: `./semaforos/linux/` Nombre: `agente2.c`, `fumadores2.h`, `fumadores2.c`, `fumadores_utilidad2.c`, `ctrl_fumadores2.c`)

* Formato: Permance igual.

* Descripción: El programa tiene muchas variables globales, las variables globales son el demonio, elimine las variables globales y haga algo similar a lo que se hizo en la implementación del jardín ornamental.
* **Nota**: No olvidar modificar el fichero `makefile`.

##### Windows

* [`fumadores.h`](./semaforos/windows/fumadores.h)
* [`agente.c`](./semaforos/windows/agente.c)
* [`fumador.c`](./semaforo/windows/fumador.c)
* [`fumadores_utilidad.c`](./semaforos/windows/fumadores_utilidade.c)
* [`ctrl_fumadores.c`](./semafores/windows/ctrl_fumadores.c)

###### Referencias

* [`CreateSemaphoreA`](https://man7.org/linux/man-pages/man7/sem_overview.7.html)
* [`WaitForSingleObject`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject)
* [`ReleaseSemaphore`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-releasesemaphore)
* [`CloseHandle`](https://learn.microsoft.com/es-es/windows/win32/api/handleapi/nf-handleapi-closehandle)
* [`OpenSemaphoreW`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-opensemaphorew)

**[Ejercicio 6].** (Carpeta: `./semaforos/windows/` Nombre: `agente2.c`, `fumadores2.h`, `fumadores2.c`, `fumadores_utilidad2.c`, `ctrl_fumadores2.c`)

* Formato: Permance igual.

* Descripción: El programa tiene muchas variables globales, las variables globales son el demonio, elimine las variables globales y haga algo similar a lo que se hizo en la implementación del jardín ornamental.
* **Nota**: No olvidar modificar el fichero `makefile`.
