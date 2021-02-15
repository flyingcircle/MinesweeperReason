open ReactNative;

let styles =
  Style.(
    StyleSheet.create({
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
  let component = (~boardRow: array(Cell.cell)) => {
    <View style=styles##rowContainer>
      {boardRow
        -> Belt.Array.map(c => {
          <Cell.component cell={c} />
        })
        -> React.array
      }
    </View>
  };
}

type row = array(Cell.cell);

[@react.component]
let component = (~board: Minesweeper.boardType) => {
  <View style=styles##container>
    {board
    -> Belt.Array.map(row => {
      <Row.component boardRow={row} />
    })
    -> React.array
    }
  </View>
};
