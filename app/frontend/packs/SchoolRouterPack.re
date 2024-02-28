open SchoolRouter__Types;
let decodeProps = json =>
  Json.Decode.(
    field("courses", array(Course.decode), json),
    field("currentUser", User.decode, json),
    field("school", School.decode, json),
  );
let (courses, currentUser, school) =
  DomUtils.parseJSONTag(~id="school-router-data", ())->decodeProps;
switch (ReactDOM.querySelector("#school-router")) {
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, <SchoolRouter school courses currentUser />);
| None => ()
};
