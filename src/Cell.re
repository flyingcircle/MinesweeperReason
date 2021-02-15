open ReactNative;

type content = 
  | Number(int)
  | Bomb
  | CEmpty;

type cellState =
  | Hidden(content)
  | Revealed(content);

type cell =
  | Selected(cellState)
  | NotSelected(cellState);

let revealCell = (c: cell): cell => {
  switch c {
    | Selected(Hidden(c)) => NotSelected(Revealed(c))
    | x => x
  };
};

let getCellState = (c: cell): cellState => {
  switch c {
    | Selected(x) => x
    | NotSelected(x) => x
  };
}

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
let component = (~cell:cell) => {
  let (isPressed, setIsPressed) = React.useState(() => false);
  let cState = getCellState(cell);
  let selectedStyle = switch cell {
    | Selected(_) => styles##selected
    | NotSelected(_) => Style.(style())
  };
  <Pressable style={Style.(array([|styles##button, selectedStyle|]))} onPress={_ => setIsPressed(_ => true)}>
    { 
      switch cState {
      | Hidden(_) => <Text>"H"->React.string</Text>
      | Revealed(Number(_)) => <Text>"N"->React.string</Text>
      | Revealed(Bomb) => <Text>"B"->React.string</Text>
      | Revealed(CEmpty) => <Text>"E"->React.string</Text>
    };}
  </Pressable>
};