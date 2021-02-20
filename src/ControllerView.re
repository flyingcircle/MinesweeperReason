open ReactNative;

let styles =
  Style.(
    StyleSheet.create({
      "container":
        viewStyle(
          ~flex=1.,
          ~flexDirection=`row,
          ~padding=15.->dp,
          ~height=100.->pct,
          ~width=100.->pct,
          ~justifyContent=`center,
          ~alignItems=`center,
          (),
        ),
      "buttonContainer":
        viewStyle(
          ~flex=3.,
          ~justifyContent=`center,
          ~alignItems=`center,
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
            ~width=100.->dp,
            ~height=100.->dp,
            ~borderRadius=50.,
            ~backgroundColor="red",
            ~margin=25.->dp,
            ~justifyContent=`center,
            ~alignItems=`center,
            (),
          ),
      })
    );

  [@react.component]
  let component = (~onPress, ~icon) => {
    <Pressable style=styles##circle onPress={onPress} >
      <RNIcons.MaterialCommunityIcons name={icon} size=80. />
    </Pressable>
  }
};

[@react.component]
let component = (~updateCB) =>
  <View style=styles##container>
    <View style=styles##buttonContainer >
      <Circle.component onPress={(_) => updateCB((0,0), Minesweeper.Flag)} icon=`_flagVariant/>
    </View>
    <View style=styles##buttonContainer >
      <Circle.component onPress={(_) => updateCB((0,0), Minesweeper.Reveal)} icon=`_pickaxe/>
    </View>
  </View>