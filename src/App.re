/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * Converted from https://github.com/facebook/react-native/blob/724fe11472cb874ce89657b2c3e7842feff04205/template/App.js
 * With a few tweaks
 */
open ReactNative;

type reactNativeNewAppScreenColors = {
  .
  "primary": string,
  "white": string,
  "lighter": string,
  "light": string,
  "black": string,
  "dark": string,
};

[@bs.module "react-native/Libraries/NewAppScreen"]
external colors: reactNativeNewAppScreenColors = "Colors";

/*
 Here is StyleSheet that is using Style module to define styles for your components
 The main different with JavaScript components you may encounter in React Native
 is the fact that they **must** be defined before being referenced
 (so before actual component definitions)
 More at https://reason-react-native.github.io/en/docs/apis/Style/
 */
let styles =
  Style.(
    StyleSheet.create({
      "appContainer":
        style(
          ~flex=1.,
          (),
        ),
      "gameContainer":
        viewStyle(
          ~flex=4.,
          ~height=100.->pct,
          ~width=100.->pct,
          ~backgroundColor=colors##primary,
          (),
        ),
      "controllerContainer":
        viewStyle(
          ~flex=2.,
          ~height=100.->pct,
          ~width=100.->pct,
          ~backgroundColor=colors##dark,
          (),
        ),
    })
  );

[@react.component]
let app = () =>
  <>
    <StatusBar barStyle=`darkContent />
    <SafeAreaView style=styles##appContainer>
      <View style=styles##gameContainer >
        <GameView.component />
      </View>
      <View style=styles##controllerContainer >
        <ControllerView.component />
      </View>
    </SafeAreaView>
  </>;
