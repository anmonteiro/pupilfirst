let parameterize = t =>
  t
  |> Js.String.toLowerCase
  |> Js.String.replaceByRe(
       ~regexp=[%mel.re "/[^0-9a-zA-Z]+/gi"],
       ~replacement="-",
     )
  |> Js.String.replaceByRe(~regexp=[%mel.re "/^-|-$/gmi"], ~replacement="");
let paramToId = param =>
  [%mel.re "/^\\d+/g"]
  ->(Js.Re.exec(~str=param))
  ->(Belt.Option.map(Js.Re.captures))
  ->(Belt.Option.map(Js.Array.join(~sep="")));
let includes = (~caseInsensitive=true, source, target) => {
  let (finalSource, finalTarget) =
    if (caseInsensitive) {
      (
        Js.String.toLocaleLowerCase(source),
        Js.String.toLocaleLowerCase(target),
      );
    } else {
      (source, target);
    };
  Js.String.includes(~search=finalSource, finalTarget);
};
let isPresent = t => Js.String.trim(t) != "";
let test = (value, text) => {
  let tester = Js.Re.fromString(value);
  Js.Re.test(tester, ~str=text);
};
let colors = [|
  ("#ff4040", false),
  ("#7f2020", false),
  ("#cc5c33", false),
  ("#734939", false),
  ("#bf9c8f", false),
  ("#995200", false),
  ("#4c2900", false),
  ("#f2a200", false),
  ("#ffd580", true),
  ("#332b1a", false),
  ("#4c3d00", false),
  ("#ffee00", true),
  ("#b0b386", false),
  ("#64664d", false),
  ("#6c8020", false),
  ("#c3d96c", true),
  ("#143300", false),
  ("#19bf00", false),
  ("#53a669", false),
  ("#bfffd9", true),
  ("#40ffbf", true),
  ("#1a332e", false),
  ("#00b3a7", false),
  ("#165955", false),
  ("#00b8e6", false),
  ("#69818c", false),
  ("#005ce6", false),
  ("#6086bf", false),
  ("#000e66", false),
  ("#202440", false),
  ("#393973", false),
  ("#4700b3", false),
  ("#2b0d33", false),
  ("#aa86b3", false),
  ("#ee00ff", false),
  ("#bf60b9", false),
  ("#4d3949", false),
  ("#ff00aa", false),
  ("#7f0044", false),
  ("#f20061", false),
  ("#330007", false),
  ("#d96c7b", false),
|];
let stringToInt = name => {
  let rec aux = (sum, remains) =>
    switch (remains) {
    | "" => sum
    | remains =>
      let firstCharacter = remains |> Js.String.slice(~start=0, ~end_=1);
      let remains = remains |> Js.String.slice(~start=1);
      aux(
        sum +. (firstCharacter |> Js.String.charCodeAt(~index=0)),
        remains,
      );
    };
  aux(0.0, name) |> int_of_float;
};
let toColor = t => {
  let index = (t |> stringToInt) mod 42;
  let (backgroundColor, blackText) = colors[index];
  (backgroundColor, if (blackText) {"#000000"} else {"#FFFFFF"});
};
let lengthBetween = (~allowBlank=true, t, min, max) => {
  let tLen = t->String.trim->String.length;
  if (tLen > 0) {
    min <= tLen || tLen >= max;
  } else {
    allowBlank;
  };
};
