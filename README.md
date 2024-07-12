# Abelian Sandpile Model

Implementation of the [Abelian sandpile model](https://en.wikipedia.org/wiki/Abelian_sandpile_model).
The program calculates model's states and saves them as BMP images at a specified frequency.

## Initial State

The initial state is defined by a TSV file that specifies the number of
sand grains in each cell, excluding empty cells. Based on this data, the grid size is calculated - the _minimal_ rectangle that encloses all the cells.

### File Format
Each line in the TSV file contains information about one cell, with the format
`x, y, number of grains in (x, y)`, separated
by tabs.
The number of grains in one cell should fit `uint64_t` range and
the coordinates should fit `int16_t` range.

## Model Details

1. New sand grains are only added during initialization.
2. The next generation of cells' state only depends on the previous grid state.
3. If a sand grain attempts to topple outside the grid's boundaries, the grid's size will increase by 1 in that direction.
4. The image of the current state is generated according to the rules:
    - The image size equals the size of the grid.
    - Each pixel corresponds to a grid cell.
    - Pixel color depends on the number of sand grains in the cell:
        + 0 - white
        + 1 - green
        + 2 - yellow
        + 3 - pink
        + \> 3 - black
    - Encoding of 1 pixel takes _no more than 4 bits_.

## Command-line Arguments

The program is a console application supporting the following command-line arguments:

- **-i, --input**: A TSV file containing initial data.
- **-o, --output**: Path to the directory to save images to.
- **-m, --max-iter**: Maximum number of toppling iterations.
- **-f, --freq**: The frequency at which images should be saved (if 0, only the final state is saved).

## Example Input

```
--input=/Users/SupercoolMe/Desktop/params.tsv --max-iter=92233720368547 -f 1000000000 -o /Users/SupercoolMe/Desktop/gallery/
```
