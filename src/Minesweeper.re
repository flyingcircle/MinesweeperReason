type action = 
  | NewGame
  | Reveal
  | Select
  | Flag;

type content = 
  | Number(int)
  | Bomb
  | CEmpty;

type cellState =
  | Hidden(content)
  | Revealed(content)
  | Flagged(content);

type cell =
  | Selected(cellState)
  | NotSelected(cellState);

type boardType = array(array(cell));
type bombList = array((int,int));

/* 
Even Not Selected cells must be revealed when an empty
space is revealed, all surrounding empty and number spaces
are also revealed as well.
*/
let revealCell = (c: cell): cell => {
  switch c {
    | Selected(Hidden(c)) => NotSelected(Revealed(c))
    | NotSelected(Hidden(c)) => NotSelected(Revealed(c))
    | x => x
  };
};

let getCellState = (c: cell): cellState => {
  switch c {
    | Selected(x) => x
    | NotSelected(x) => x
  };
};

let getCellContent = (c: cell): content => {
  let cs = getCellState(c)
  switch cs {
    | Hidden(x) => x
    | Revealed(x) => x
    | Flagged(x) => x
  }
}

/*
The pivotal higher order function, takes a function that describes
how to build a new board.
*/
let createBoard = (f): boardType => {
  Belt.Array.map( Belt.Array.range(0,9), y => Belt.Array.map(
  Belt.Array.range(0,9), x => f(x, y)));
}

/*
Takes a coordinate on the board and updates the board using:
u: a function used to update at the specified coord (x,y).
s: a function used to update every other poistion.
*/
let updateCell = ((x, y): (int, int), b: boardType, u: cell => cell, s: cell => cell) => {
  createBoard((x',y') => {
    let cell: cell = Array.get(Array.get(b, y'), x')
    if (x == x' && y == y')
      { u(cell); }
    else
      { s(cell); }
  });
}

// used in dev.
let emptyBoard = createBoard((_, _) => NotSelected(Hidden(CEmpty)));

// required in order to curry below
let eq = (x, y) => x == y;

// randomly create x number of coordinates.
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

// in order create the Number() cells, we need to count bombs around a 
// particular cell
let getSurroundingCoords = ((x,y): (int, int)): array((int,int)) => {
  let offset = [|(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)|];
  Array.map(((x',y')) => (x + x', y + y'), offset)
}

module PairComparator =
  Belt.Id.MakeComparable({
    type t = (int, int);
    let cmp = ((a0, a1), (b0, b1)) =>
      switch (Pervasives.compare(a0, b0)) {
      | 0 => Pervasives.compare(a1, b1)
      | c => c
      };
  });

// The Number(x) of a cell = count (bomb-coords ∩ surrounding-cells).
let numberSurroundingBombs = (coord: (int, int), bombList): int => {
  let bombSet = Belt.Set.fromArray(bombList, ~id=(module PairComparator));
  let surroundingSet = Belt.Set.fromArray(getSurroundingCoords(coord), ~id=(module PairComparator));
  Belt.Set.intersect(bombSet, surroundingSet)->Belt.Set.size;
}

/*
Gen new bomb list and derive the rest of the board from there.
*/
let newGame = (): boardType => {
  Random.init(int_of_float(Js.Date.now()));
  let bombList = generateBombPlacement(10, [||]);
  createBoard((x, y) => switch (x, y) {
    | (x, y) when Array.exists(eq((x,y)), bombList) => NotSelected(Hidden(Bomb))
    | (x, y) when numberSurroundingBombs((x,y), bombList) > 0 => NotSelected(Hidden(Number(numberSurroundingBombs((x,y), bombList))))
    | _ => NotSelected(Hidden(CEmpty))
  });
}

/*
Next 2 functions are mutually recursive. (ReasonML was giving an
error when I attempted to put a switch inside another switch).

When a spot is revealed, we need to check if we revealed an empty
square. If so, we recurse down the surrounding cells to reveal all
adjacent empty and number squares.  
*/
let rec revealSpace = ((x,y): (int, int), b: boardType): boardType => {
  switch (x, y) {
    | (x, _) when x < 0 || x > 9 => b
    | (_, y) when y < 0 || y > 9 => b
    | _ => revealEmptiesAt((x,y), b);
  }
}
and revealEmptiesAt = ((x, y): (int, int), b: boardType): boardType => {
  let cell: cell = Array.get(Array.get(b, y), x);
  let cs = getCellState(cell);
  switch cs {
    | Hidden(CEmpty) => {
      let b' = updateCell((x,y), b, revealCell, x => x);
      Array.fold_left((a,b) => revealSpace(b,a), b', [|(x+1, y), (x-1, y), (x, y+1), (x, y-1)|]);
    }
    // Since no empty square touches a bomb, we are safe to use an '_' here.
    | Hidden(_) => updateCell((x,y), b, revealCell, x => x);
    | _ => b
  };
}

let selectCell = (coord: (int, int), b: boardType) => {
  updateCell(coord, b, 
    c => Selected(getCellState(c)), 
    c => NotSelected(getCellState(c)));
};

let setFlag = (coord: (int, int), b:boardType) => {
  updateCell(coord, b,
    c => switch c {
      | Selected(Flagged(_)) => Selected(Hidden(getCellContent(c)))
      | _ => Selected(Flagged(getCellContent(c)))
    },
    c => c);
}

let won = (b: boardType): bool => {
  let fold_func = (v, c) => {
    let cs = getCellState(c);
    v && switch cs {
      | Hidden(Bomb) => true
      | Flagged(Bomb) => true
      | Revealed(Number(_)) => true
      | Revealed(CEmpty) => true
      | _ => false
  }};
  Array.fold_left(fold_func, true, Array.fold_left(Array.append, [||], b));
}

let lost = (b: boardType): bool => {
  let fold_func = (v, c) => {
    let cs = getCellState(c);
    v || switch cs {
      | Revealed(Bomb) => true
      | _ => false
  }};
  Array.fold_left(fold_func, false, Array.fold_left(Array.append, [||], b));
};
