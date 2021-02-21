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
        "smaller":
          viewStyle(
            ~width=50.->dp,
            ~height=50.->dp,
            ~borderRadius=25.,
            (),
          ),
      })
    );

  [@react.component]
  let component = (~onPress, ~icon, ~size=80., ~smaller=false) => {
    let smallStyle = smaller ? styles##smaller : Style.(style());
    <Pressable style={Style.(array([|styles##circle, smallStyle|]))} onPress={onPress} >
      <RNIcons.MaterialCommunityIcons name={icon} size={size} />
    </Pressable>
  }
};

[@react.component]
let component = (~updateCB) =>
  <>
    <View style=styles##container>
      <View style=styles##buttonContainer >
        <Circle.component onPress={(_) => updateCB((0,0), Minesweeper.Flag)} icon=`_flagVariant/>
      </View>
      <View style=styles##buttonContainer >
        <Circle.component onPress={(_) => updateCB((0,0), Minesweeper.Reveal)} icon=`_pickaxe/>
      </View>
    </View>
    <Circle.component onPress={_ => updateCB((0,0), Minesweeper.NewGame)} icon=`_newBox size=40. smaller=true />
  </>
