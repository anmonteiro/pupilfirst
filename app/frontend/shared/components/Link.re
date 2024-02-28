let ctrlKey = Webapi.Dom.KeyboardEvent.ctrlKey;
let metaKey = Webapi.Dom.KeyboardEvent.metaKey;
external unsafeAsKeyboardEvent:
  React.Event.Mouse.t => Webapi.Dom.KeyboardEvent.t =
  "%identity";
let onConfirm = (href, onClick, event) => {
  React.Event.Mouse.preventDefault(event);
  Belt.Option.mapWithDefault(onClick, (), onClick => onClick(event));
  ReasonReactRouter.push(href);
};
let onCancel = event => event |> React.Event.Mouse.preventDefault;
let handleOnClick = (href, confirm, onClick, event) => {
  let keyboardEvent = event |> unsafeAsKeyboardEvent;
  let modifierPressed = keyboardEvent |> ctrlKey || keyboardEvent |> metaKey;
  switch (modifierPressed, confirm) {
  | (true, _) => ()
  | (false, Some(confirmationText)) =>
    WindowUtils.confirm(
      ~onCancel=() => onCancel(event),
      confirmationText,
      () => onConfirm(href, onClick, event),
    )
  | (false, None) => onConfirm(href, onClick, event)
  };
};
let link = (href, includeSearch) => {
  let search =
    Webapi.Dom.window->Webapi.Dom.Window.location->Webapi.Dom.Location.search;

  if (includeSearch) {
    href ++ search;
  } else {
    href;
  };
};
[@react.component]
let make =
    (
      ~href,
      ~ariaLabel=?,
      ~className=?,
      ~confirm=?,
      ~id=?,
      ~onClick=?,
      ~title=?,
      ~children,
      ~includeSearch=false,
      ~disabled=false,
      ~props=?,
    ) => {
  let switchProps = Belt.Option.getWithDefault(props, Js.Obj.empty());
  <Spread props=switchProps>
    <a
      href={link(href, includeSearch)}
      ?ariaLabel
      ?className
      ?id
      ?title
      onClick={handleOnClick(link(href, includeSearch), confirm, onClick)}
      disabled>
      children
    </a>
  </Spread>;
};
