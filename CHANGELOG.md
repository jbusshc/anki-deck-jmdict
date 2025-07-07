# Changelog

## TODO

- Añadir clase para Kanjis, para incluirlos en cada entrada.

## [0.9.0] - 2025-7-6

### Added
- Routes as constants
- Misc in sense (anki card)
- Ant y ref (anki card)

## [0.8.0] - 2025-3-15

### Added
- Banderas que diferencian idiomas.
- Palabra se resalta en la oración de ejemplo.

### Fixed
- CSS correcto para separar oraciones con ejemplos.


## [0.7.1] - 2025-3-10

### Fixed 
- Ejemplos ahora se extraen correctamente para cada sense desde otro xml.


## [0.7.0] - 2025-2-16

### Changed
Se cambió el modelo: entradas JMDictEntry pasan a ser std::vector<JMDictEntry>> 

### Fixed
- Se separan correctamente las entradas con múltiples lecturas.

### Removed
- Se elimina la función mergeEntry (ya no es necesaria)
- Se han eliminado temporalmente los ejemplos de las tarjetas


## [0.6.2] - 2024-12-6

### Added 
- Añadida función cleanText,
- Añadido código para parsear sin enteros el xml.



## [0.6.1] - 2024-12-5

### Added 
- Modo para solo recuperar ejemplos en Sense.
### Fixed
- Quitar elementos repetidos en merge entry.


## [0.6.0] - 2024-12-2
### Added
- Reescritura de código completo.
- Añadir ejemplos al mazo de anki al momento de generarlo.

## [0.5.0] - 2024-09-28

### Added
- Se fusionan las cartas que comparten la misma clave primaria.

## [0.4.0] - 2024-09-19

### Added
- Creada clase example
- Se añaden los ejemplos a las entradas.

## [0.3.0] - 2024-09-18

### Added
- Se crea clase generadora de cartas Anki.
- Se crea el método de apertura: IMPORT.
- Se crea el método de apertura: EXAMPLE_MERGE.

### Changed
- Se mejora la función exportadora para que funcione acorde a import.

## [0.2.0] - 2024-09-14

### Added
- Se termina de procesar el diccionario.
- Permite seleccionar a qué idioma extraer gloss (uno o más).
- Extrae correctamente todos los items en sense.

## [0.1.0] - 2024-09-11

### Added
- Commit Inicial
- Se crea la clase JMDict para procesar el XML.
