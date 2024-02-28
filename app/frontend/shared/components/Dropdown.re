open Webapi.Dom;
let onWindowClick = (showDropdown, setShowDropdown, _event) =>
  if (showDropdown) {
    setShowDropdown(_ => false);
  } else {
    ();
  };
let toggleDropdown = (setShowDropdown, event) => {
  event |> React.Event.Mouse.stopPropagation;
  setShowDropdown(showDropdown => !showDropdown);
};
let containerClasses = className =>
  "dropdown inline-block relative text-sm " ++ className;
[@react.component]
let make = (~selected, ~contents, ~right=false, ~className="w-full md:w-auto") => {
  let (showDropdown, setShowDropdown) = React.useState(() => false);
  React.useEffect1(
    () => {
      let curriedFunction = onWindowClick(showDropdown, setShowDropdown);
      let removeEventListener = () =>
        Window.removeEventListener("click", curriedFunction, window);
      if (showDropdown) {
        Window.addEventListener("click", curriedFunction, window);
        Some(removeEventListener);
      } else {
        removeEventListener();
        None;
      };
    },
    [|showDropdown|],
  );
  <div
    className={containerClasses(className)}
    onClick={toggleDropdown(setShowDropdown)}>
    selected
    {if (showDropdown) {
       <div
         className={
           "dropdown__list bg-white shadow-lg rounded mt-1 border border-gray-300 divide-y divide-gray-50 absolute overflow-x-hidden z-30 "
           ++ (if (right) {"right-0"} else {"left-0"})
         }>
         {contents
          |> Array.mapi((index, content) =>
               <div
                 key={"dropdown-" ++ (index |> string_of_int)}
                 className="cursor-pointer block text-sm font-medium text-gray-900 bg-white hover:text-primary-500 hover:bg-gray-50 focus-within:outline-none focus-within:bg-gray-50 focus-within:text-primary-500">
                 content
               </div>
             )
          |> React.array}
       </div>;
     } else {
       React.null;
     }}
  </div>;
};
