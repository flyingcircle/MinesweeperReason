open ReactNative;

let styles =
  Style.(
    StyleSheet.create({
      "gameContainer":
        viewStyle(
          ~flex=4.,
          ~height=100.->pct,
          ~width=100.->pct,
          (),
        ),
      "controllerContainer":
        viewStyle(
          ~flex=2.,
          ~height=100.->pct,
          ~width=100.->pct,
          ~backgroundColor="grey",
          (),
        ),
      "container":
        viewStyle(
          ~flex=1.,
          ~padding=20.->dp,
          ~height=100.->pct,
          ~width=100.->pct,
          ~justifyContent=`center,
          ~alignItems=`center,
          (),
        ),
      "rowContainer":
        viewStyle(
          ~flexDirection=`row,
          (),
        ),
      "columnContainer":
        viewStyle(
          ~flexDirection=`column,
          ~flex=1.,
          ()
        ),
    })
  );

module Row = {
  [@react.component]
  let component = (~boardRow: array(Minesweeper.cell), ~rowIndex: int, ~updateCB) => {
    <View style=styles##rowContainer>
      {boardRow
        -> Belt.Array.mapWithIndex((i, c) => {
          <Cell.component cell={c} key={Js.Int.toString(i)} x={i} y={rowIndex} updateCB={updateCB} />
        })
        -> React.array
      }
    </View>
  };
}

type row = array(Minesweeper.cell);

[@react.component]
let component = () => {
  let (board, setBoard) = React.useState(() => Minesweeper.newGame);
  let (selected, setSelected) = React.useState(() => (-1,-1));
  let getSelected = () => selected;
  let updateBoard = (coord: (int,int), a: Minesweeper.action) => {
    switch a {
      | Reveal => setBoard(board => Minesweeper.revealSpace(getSelected(), board))
      | Select => {
          setSelected(_ => {
            coord});
          setBoard(board => Minesweeper.selectCell(coord, board));
      }
      | Flag => setBoard(board => board)
    };
  };
  <>
    <View style=styles##gameContainer >
      <View style=styles##container>
        {board
        -> Belt.Array.mapWithIndex((i, row) => {
          <Row.component key={Js.Int.toString(i)} boardRow={row} rowIndex={i} updateCB={updateBoard} />
        })
        -> React.array
        }
      </View>
    </View>
    <View style=styles##controllerContainer >
      <ControllerView.component updateCB={updateBoard} />
    </View>
  </>
};
