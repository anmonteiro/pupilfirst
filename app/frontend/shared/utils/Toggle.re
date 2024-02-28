let context = React.createContext([||]: array(string));
let enabled = key => {
  let toggles = React.useContext(context);
  Js.Array.some(toggles, ~f=x => x == key);
};
module Provider = {
  let provider = React.Context.provider(context);
  [@react.component]
  let make = (~value, ~children) =>
    React.createElement(provider, [%obj {value, children}]);
};
