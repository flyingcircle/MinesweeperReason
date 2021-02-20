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
      "messageContainer":
        viewStyle(
          ~height=50.->dp,
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
      "winMessage":
        style(
          ~fontSize=30.,
          ~fontWeight=`_600,
          ~color="green",
          (),
        ),
      "loseMessage":
        style(
          ~fontSize=30.,
          ~fontWeight=`_600,
          ~color="red",
          (),
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
  let (board, setBoard) = React.useState(Minesweeper.newGame);
  let (selected, setSelected) = React.useState(() => (-1,-1));
  let (won, setWon) = React.useState(() => false);
  let (lost, setLost) = React.useState(() => false);
  let updateBoard = (coord: (int,int), a: Minesweeper.action) => {
    switch a {
      | Reveal => {
        setBoard(board => {
          let b' = Minesweeper.revealSpace(selected, board)
          setWon(_ => Minesweeper.won(b'));
          setLost(_ => Minesweeper.lost(b'));
          b'
        });
      }
      | Select => {
          setSelected(_ => coord);
          setBoard(board => Minesweeper.selectCell(coord, board));
      }
      | Flag => setBoard(board => Minesweeper.setFlag(selected, board))
      | NewGame => {
        setWon(_ => false);
        setLost(_ => false);
        setBoard(_ => Minesweeper.newGame());
      }
    };
  };
  <>
    <View style=styles##gameContainer>
      <View style=styles##container>
        <View style=styles##messageContainer>
          {won ?
            (<Text style=styles##winMessage >"You Won!"->React.string</Text>) :
            (<View/>)
          }
          {lost ?
            (<Text style=styles##loseMessage >"You Lost! :("->React.string</Text>) :
            (<View/>)
          }
        </View>
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
