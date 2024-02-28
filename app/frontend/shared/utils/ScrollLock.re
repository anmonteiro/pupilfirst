open Webapi.Dom;
let handleScrollLock = add => {
  let classes = if (add) {"overflow-hidden"} else {""};
  let body =
    document
    |> Document.getElementsByTagName("body")
    |> HtmlCollection.toArray;
  body[0]->(Element.setClassName(classes));
};
let activate = () => handleScrollLock(true);
let deactivate = () => handleScrollLock(false);
