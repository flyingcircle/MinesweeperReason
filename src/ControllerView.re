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
    })
  );

[@react.component]
let component = () =>
  <View style=styles##container>
    <Text>"Buttons go here" -> React.string </Text>
  </View>