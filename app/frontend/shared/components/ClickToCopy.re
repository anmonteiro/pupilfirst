let str = React.string;
let t = I18n.t(~scope="components.ClickToCopy");
type nonrec state =
  | ReadyToCopy
  | Copied;
let performCopy = (_copy, setState, _event) => {
  [%raw {js|navigator.clipboard.writeText(_copy)|js}]->ignore;
  setState(_ => Copied);
};
let refresh = (setState, _event) => setState(_ => ReadyToCopy);
[@react.component]
let make =
    (
      ~copy,
      ~tooltipPosition=`Top,
      ~className="",
      ~tooltipClassName=?,
      ~children,
    ) => {
  let (state, setState) = React.useState(() => ReadyToCopy);
  let tip =
    (
      switch (state) {
      | ReadyToCopy => t("copy")
      | Copied => t("copied")
      }
    )
    ->str;
  <div
    className={"cursor-pointer " ++ className}
    onClick={performCopy(copy, setState)}
    onMouseLeave={refresh(setState)}>
    <Tooltip className=?tooltipClassName position=tooltipPosition tip>
      children
    </Tooltip>
  </div>;
};
