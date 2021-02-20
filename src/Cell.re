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
        ),
      "hidden":
        viewStyle(
          ~height=100.->pct,
          ~width=100.->pct,
          ~backgroundColor="grey",
          (),
        ),
      "empty":
        viewStyle(
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
      | Hidden(_) => <View style=styles##hidden/>
      | Flagged(_) => <RNIcons.MaterialCommunityIcons name=`_flagVariant size=15. />
      | Revealed(Number(a)) => <Text>{Js.Int.toString(a)}->React.string</Text>
      | Revealed(Bomb) => <RNIcons.MaterialCommunityIcons name=`_bomb size=15. />
      | Revealed(CEmpty) => <View style=styles##empty/>
      };
    } 
  </Pressable>
};