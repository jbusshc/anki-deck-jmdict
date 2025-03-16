# Changelog

## [0.8.0] - 2022-3-15

## Added
- Banderas que diferencian idiomas.
- Palabra se resalta en la oración de ejemplo.

## Fixed
- CSS correcto para separar oraciones con ejemplos.

### Pending
- Añadir clase para Kanjis, para incluirlos en cada entrada.
- Ant y ref en anki


## [0.7.1] - 2022-3-10

## Fixed 
- Ejemplos ahora se extraen correctamente para cada sense desde otro xml.

### Pending
- Añadir clase para Kanjis, para incluirlos en cada entrada.
- Mostrar diferencias de idioma.
- Ant y ref en anki

## [0.7.0] - 2022-2-16

## Changed
Se cambió el modelo: entradas JMDictEntry pasan a ser std::vector<JMDictEntry>> 

## Fixed
- Se separan correctamente las entradas con múltiples lecturas.

## Removed
- Se elimina la función mergeEntry (ya no es necesaria)
- Se han eliminado temporalmente los ejemplos de las tarjetas

### Pending
- Añadir ejemplos de las palabras
- Añadir clase para Kanjis, para incluirlos en cada entrada.
- Mostrar diferencias de idioma.
- Ant y ref en anki

## [0.6.2] - 2024-12-6
### Added 
- Añadida función cleanText,
- Añadido código para parsear sin enteros el xml.
### Pending
- Mejorar .css de Anki.
- Añadir clase para Kanjis, para incluirlos en cada entrada.
- Mostrar diferencias de idioma.
- Ant y ref en anki


## [0.6.1] - 2024-12-5
### Added 
- Modo para solo recuperar ejemplos en Sense.
### Fixed
- Quitar elementos repetidos en merge entry.
### Pending
- Mejorar .css de Anki.
- Añadir clase para Kanjis, para incluirlos en cada entrada.
- Mostrar diferencias de idioma.
- Ant y ref en anki

## [0.6.0] - 2024-12-2
### Added
- Reescritura de código completo.
- Añadir ejemplos al mazo de anki al momento de generarlo.

### Pending
- Mejorar .css de Anki.
- Añadir clase para Kanjis, para incluirlos en cada entrada.
- Mostrar diferencias de idioma.

## [0.5.0] - 2024-09-28
### Added
- Se fusionan las cartas que comparten la misma clave primaria.

### Pending
- Mejorar .css de Anki.
- Añadir clase para Kanjis, para incluirlos en cada entrada.
- Mostrar diferencias de idioma.
- Añadir ejemplos al mazo de anki al momento de generarlo.

## [0.4.0] - 2024-09-19

### Added
- Creada clase example
- Se añaden los ejemplos a las entradas.

### Pending
- Revisar si hay cartas que comparten la misma clave primaria.
- Mejorar .css de Anki.
- Añadir clase para Kanjis, para incluirlos en cada entrada.
- Mostrar diferencias de idioma.
- Añadir ejemplos al mazo de anki al momento de generarlo.


## [0.3.0] - 2024-09-18

### Added
- Se crea clase generadora de cartas Anki.
- Se crea el método de apertura: IMPORT.
- Se crea el método de apertura: EXAMPLE_MERGE.

### Changed
- Se mejora la función exportadora para que funcione acorde a import.

### Pending
- Mejorar .css de Anki.
- U̶t̶i̶l̶i̶z̶a̶r̶ ̶u̶n̶a̶ ̶E̶D̶ ̶q̶u̶e̶ ̶p̶e̶r̶m̶i̶t̶a̶ ̶b̶ú̶s̶q̶u̶e̶d̶a̶ ̶e̶n̶ ̶l̶o̶g̶n.
- Mostrar diferencias de idioma.
- Crear clase example.
- Añadir ejemplos de oraciones para las palabras.

## [0.2.0] - 2024-09-14

### Added
- Se termina de procesar el diccionario.
- Permite seleccionar a qué idioma extraer gloss (uno o más).
- Extrae correctamente todos los items en sense.

### Pending
- Crear la clase generadora de cartas Anki.

## [0.1.0] - 2024-09-11

### Added
- Commit Inicial
- Se crea la clase JMDict para procesar el XML.

### Pending 
- Cuando se procesa el gloss se añaden todos los idiomas, falta filtrar.
- Verificar el resto de items en Sense para cada entrada.
