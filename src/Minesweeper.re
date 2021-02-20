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

let createBoard = (f): boardType => {
  Belt.Array.map( Belt.Array.range(0,9), y => Belt.Array.map(
  Belt.Array.range(0,9), x => f(x, y)));
}

let updateCell = ((x, y): (int, int), b: boardType, u: cell => cell, s: cell => cell) => {
  createBoard((x',y') => {
    let cell: cell = Array.get(Array.get(b, y'), x')
    if (x == x' && y == y')
      { u(cell); }
    else
      { s(cell); }
  });
}

let emptyBoard = createBoard((_, _) => NotSelected(Hidden(CEmpty)));

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

let numberSurroundingBombs = (coord: (int, int), bombList): int => {
  let bombSet = Belt.Set.fromArray(bombList, ~id=(module PairComparator));
  let surroundingSet = Belt.Set.fromArray(getSurroundingCoords(coord), ~id=(module PairComparator));
  Belt.Set.intersect(bombSet, surroundingSet)->Belt.Set.size;
}

let newGame = (): boardType => {
  Random.init(int_of_float(Js.Date.now()));
  let bombList = generateBombPlacement(10, [||]);
  createBoard((x, y) => switch (x, y) {
    | (x, y) when Array.exists(eq((x,y)), bombList) => NotSelected(Hidden(Bomb))
    | (x, y) when numberSurroundingBombs((x,y), bombList) > 0 => NotSelected(Hidden(Number(numberSurroundingBombs((x,y), bombList))))
    | _ => NotSelected(Hidden(CEmpty))
  });
}

let rec revealSpace = ((x,y): (int, int), b: boardType): boardType => {
  Js.log({j|revealing space: $x, $y|j});
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
