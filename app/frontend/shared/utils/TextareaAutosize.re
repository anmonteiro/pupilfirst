open Webapi.Dom;
[@mel.module "autosize"]
external autosizeFunction: Dom.element => unit = "default";
type nonrec autosize;
[@mel.module "autosize"] external autosizeModule: autosize = "default";
[@mel.send]
external autosizeDestroy: (autosize, Dom.element) => unit = "destroy";
[@mel.send]
external autosizeUpdate: (autosize, Dom.element) => unit = "update";
let perform = (f, id) =>
  document
  |> Document.getElementById(id)
  |> OptionUtils.mapWithDefault(element => element |> f, ());
let create = id => id |> perform(autosizeFunction);
let update = id => id |> perform(autosizeUpdate(autosizeModule));
let destroy = id => id |> perform(autosizeDestroy(autosizeModule));
