open ReactNative;

let styles =
  Style.(
    StyleSheet.create({
      "container":
        viewStyle(
          ~flex=1.,
          ~padding=15.->dp,
          ~height=100.->pct,
          ~width=100.->pct,
          ~justifyContent=`center,
          ~alignItems=`center,
          (),
        ),
      "leftRightContainer":
        viewStyle(
          ~flex=1.,
          ~flexDirection=`row,
          (),
        ),
    })
  );

module Arrow = {

  type direction = Up | Left | Right | Down;

  let styles =
    Style.(
      StyleSheet.create({
        "triangle":
          viewStyle(
            ~width=0.->pct,
            ~height=0.->pct,
            ~backgroundColor="transparent",
            ~borderStyle=`solid,
            ~borderLeftWidth=20.,
            ~borderRightWidth=20.,
            ~borderBottomWidth=40.,
            ~borderLeftColor="transparent",
            ~borderRightColor="transparent",
            ~borderBottomColor="red",
            (),
          ),
        "down":
          viewStyle(
            ~transform=[| 
              rotate(~rotate=180.->deg)
            |],
            (),
          ),
        "left":
          viewStyle(
            ~transform=[| 
              rotate(~rotate=270.->deg)
            |],
            (),
          ),
        "right":
          viewStyle(
            ~transform=[| 
              rotate(~rotate=90.->deg)
            |],
            (),
          ),
        "up":
          viewStyle(
            ~transform=[| 
              rotate(~rotate=0.->deg)
            |],
            (),
          ),
      })
    );

  [@react.component]
  let component = (~direction: direction) => {
    let transformStyle = 
      switch direction {
        | Down => styles##down
        | Left => styles##left
        | Right => styles##right
        | Up => styles##up
      };
    (<View style=Style.(array([|styles##triangle, transformStyle|])) >
    </View>);
  }
};

module Circle = {
  let styles =
    Style.(
      StyleSheet.create({
        "circle":
          viewStyle(
            ~width=50.->dp,
            ~height=50.->dp,
            ~borderRadius=50.,
            ~backgroundColor="red",
            ~margin=25.->dp,
            (),
          ),
      })
    );

  [@react.component]
  let component = (~onPress) => {
    <Pressable style=styles##circle onPress={onPress} />
  }
};

[@react.component]
let component = (~updateCB) =>
  <View style=styles##container>
    <Arrow.component direction={Arrow.Up} />
    <View style=styles##leftRightContainer >
      <Arrow.component direction={Arrow.Left} />
      <Circle.component onPress={(_) => updateCB((0,0), Minesweeper.Reveal)}/>
      <Arrow.component direction={Arrow.Right} />
    </View>
    <Arrow.component direction={Arrow.Down} />
  </View>