open Webapi.Dom;

[@mel.scope "Document"] external readyState: string = "readyState";

let log = Debug.log("PSJ");

let ready = f =>
  if (readyState != "loading") {
    f();
  } else {
    Document.addEventListener("DOMContentLoaded", _event => f(), document);
  };

let match = (~onReady=true, path, f) => {
  log("Try to match " ++ path);

  let pathFragments = Js.String.split(path, ~sep="#");

  if (pathFragments->Js.Array.length != 2) {
    Debug.error(
      "PSJ",
      "Path must be of the format `controller#action` or `module/controller#action`. Received: "
      ++ path,
    );
  } else {
    let metaTag = document |> Document.querySelector("meta[name='psj']");

    switch (metaTag) {
    | None => ()
    | Some(tag) =>
      let controller = Element.getAttribute("controller", tag);
      let action = Element.getAttribute("action", tag);

      switch (controller, action) {
      | (Some(controller), Some(action)) =>
        if (controller == pathFragments[0] && action == pathFragments[1]) {
          log("Matched " ++ path);

          if (onReady) {
            ready(f);
          } else {
            f();
          };
        }
      | (None, Some(_)) =>
        Debug.error("PSJ", "Meta tag is missing the controller prop.")
      | (Some(_), None) =>
        Debug.error("PSJ", "Meta tag is missing the action prop.")
      | (None, None) =>
        Debug.error(
          "PSJ",
          "Meta tag is missing both the controller or action prop.",
        )
      };
    };
  };
};

let sanitizePath = path =>
  path
  ->(Js.String.replaceByRe(~regexp=[%re "/^\\//"], ~replacement=""))
  ->(Js.String.replaceByRe(~regexp=[%re "/\\/$/"], ~replacement=""));

let matchPaths = (~onReady=true, paths, f) => {
  log("Try to match paths " ++ Js.Array.join(paths, ~sep=", "));

  let _ =
    Js.Array.some(
      paths,
      ~f=path => {
        let pathFragments = Js.String.split(path, ~sep="/");
        let currentPathFragments =
          Location.pathname(location)
          ->sanitizePath
          ->(Js.String.split(~sep="/"));

        if (Js.Array.length(pathFragments)
            == Js.Array.length(currentPathFragments)) {
          let matched =
            Js.Array.everyi(pathFragments, ~f=(fragment, index) =>
              if (fragment == "*"
                  || Js.String.charAt(fragment, ~index=0) == ":") {
                true;
              } else if (fragment == currentPathFragments[index]) {
                true;
              } else {
                false;
              }
            );

          if (matched) {
            log("Matched " ++ path);

            if (onReady) {
              ready(f);
            } else {
              f();
            };
          };

          matched;
        } else {
          false;
        };
      },
    );
  ();
};
