type nonrec config = {debug: option(bool)};
[@mel.scope "window"] external config: config = "pupilfirst";
let log = (scope: string, message: string) =>
  if (Belt.Option.getWithDefault(config.debug, false)) {
    Js.log((("[" ++ scope) ++ "] ") ++ message);
  };
let error = (scope: string, message: string) =>
  Js.Console.error((("[" ++ scope) ++ "] ") ++ message);
