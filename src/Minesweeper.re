

type boardType = array(array(Cell.cell));
type bombList = array((int,int))

Random.init(int_of_float(Js.Date.now()));

let createBoard = (f): boardType => {
  Belt.Array.map( Belt.Array.range(1,10), y => Belt.Array.map(
  Belt.Array.range(1,10), x => f(x, y)));
}

let emptyBoard = createBoard((_, _) => Cell.NotSelected(Cell.Hidden(Cell.CEmpty)));

let eq = (x, y) => x == y;

let rec generateBombPlacement = (remaining: int, bombCoordinates: bombList) => {
  if (remaining == 0)
    { bombCoordinates; }
  else
    { let coord = (Random.int(10), Random.int(10));
      if (Array.exists(eq(coord), bombCoordinates))
        { generateBombPlacement(remaining, bombCoordinates);}
      else
        { generateBombPlacement(remaining-1, Belt.Array.concat([|coord|], bombCoordinates));}
    }
}

let newGame: boardType = {
  let bombList = generateBombPlacement(10, [||]);
  createBoard((x, y) => switch (x, y) {
    | (x, y) when Array.exists(eq((x,y)), bombList) => Cell.NotSelected(Cell.Hidden(Cell.Bomb))
    | _ => Cell.NotSelected(Cell.Hidden(Cell.CEmpty))
  });
}

let revealSpace = ((x, y): (int, int), b: boardType) => {
  createBoard((x',y') => {
    if (x == x' && y == y')
      { Cell.revealCell(Array.get(Array.get(b, y), x)); }
    else
      { Array.get(Array.get(b, y), x) }
  });
};

let selectCell = (c: Cell.cell, b: boardType) => {
  createBoard((x,y) => {
    let c' = Array.get(Array.get(b, y), x);
    switch c' {
      | s when c' == c => Selected(Cell.getCellState(s))
      | s => NotSelected(Cell.getCellState(s))
    };
  });
}

let inProgress = (b: boardType) => true;

let won = (b: boardType) => true;

let lost = (b: boardType) => false;
