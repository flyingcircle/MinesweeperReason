open ReactNative;

let styles =
  Style.(
    StyleSheet.create({
      "button":
        style(
          ~width=30.->dp,
          ~height=30.->dp,
          ~borderWidth=2.,
          ~justifyContent=`center,
          ~alignItems=`center,
          (),
        ),
      "selected":
        style(
          ~borderWidth=5.,
          ~borderColor="red",
          (),
        )
    })
  );

[@react.component]
let component = (~cell:Minesweeper.cell, ~x:int, ~y:int, ~updateCB) => {
  let cState = Minesweeper.getCellState(cell);
  let selectedStyle = switch cell {
    | Selected(_) => styles##selected
    | NotSelected(_) => Style.(style())
  };
  let onPress = (_) => {
    updateCB((x,y), Minesweeper.Select)
  };
  <Pressable style={Style.(array([|styles##button, selectedStyle|]))} onPress={onPress}>
    { 
      switch cState {
      | Hidden(_) => <Text>"H"->React.string</Text>
      | Flagged(_) => <Text>"F"->React.string</Text>
      | Revealed(Number(a)) => <Text>{Js.Int.toString(a)}->React.string</Text>
      | Revealed(Bomb) => <Text>"B"->React.string</Text>
      | Revealed(CEmpty) => <Text>"E"->React.string</Text>
      };
    } 
  </Pressable>
};