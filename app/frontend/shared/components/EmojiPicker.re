open Webapi.Dom;
type nonrec emojiEvent = {
  id: string,
  native: string,
  unifield: string,
  shortcodes: string,
};
type nonrec args = {
  title: string,
  ref: React.ref(Js.Nullable.t(Dom.element)),
  theme: string,
  onEmojiSelect: emojiEvent => unit,
  data: Js.Json.t,
};
[@mel.module "emoji-mart"] [@mel.new] external picker: args => unit = "Picker";
[@mel.module "@emoji-mart/data"] external data: Js.Json.t = "default";
let emojiDivClassName = isOpen =>
  isOpen
    ? "absolute top-full -left-32 md:-translate-x-0 z-10 shadow " : "hidden";
[@react.component]
let make = (~className, ~title, ~onChange) => {
  let ref = React.useRef(Js.Nullable.null);
  let wrapperRef = React.useRef(Js.Nullable.null);
  let (isOpen, setIsOpen) = React.useState(_ => false);
  React.useEffect0(() => {
    picker({
      title,
      ref,
      theme: "light",
      data,
      onEmojiSelect: event => {
        onChange(event);
        setIsOpen(_ => false);
      },
    });
    let handleClickOutside: Dom.mouseEvent => unit =
      event => {
        switch (wrapperRef.current->Js.Nullable.toOption) {
        | Some(wrapper) =>
          if (!(
                wrapper
                |> Element.contains(
                     event |> MouseEvent.target |> EventTarget.unsafeAsElement,
                   )
              )) {
            setIsOpen(_ => false);
          }
        | None => ()
        };
        ();
      };
    let handleEscKey: Dom.keyboardEvent => unit =
      e => {
        let key = e |> KeyboardEvent.key;
        if (key == "Escape" || key == "Esc") {
          setIsOpen(_ => false);
        };
        ();
      };
    document |> Document.addKeyUpEventListener(handleEscKey);
    document |> Document.addClickEventListener(handleClickOutside);
    Some(
      () => {
        document |> Document.removeKeyUpEventListener(handleEscKey);
        document |> Document.removeClickEventListener(handleClickOutside);
      },
    );
  });
  <div
    className="sm:relative inline-block"
    ref={ReactDOM.Ref.domRef(wrapperRef)}>
    <button
      ariaLabel=title title onClick={_ => setIsOpen(prev => !prev)} className>
      <i className="fas fa-smile" />
    </button>
    <div className={"transition-all " ++ emojiDivClassName(isOpen)}>
      <div ref={ReactDOM.Ref.domRef(ref)} />
    </div>
  </div>;
};
