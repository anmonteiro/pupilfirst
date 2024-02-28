open CourseAuthors__Types;
let decodeProps = json =>
  Json.Decode.(
    json |> field("courseId", string),
    json |> field("authors", array(Author.decode)),
  );
Psj.matchPaths(
  [|
    "school/courses/:course_id/authors",
    "school/courses/:course_id/authors/:author_id",
    "school/courses/:course_id/authors/new",
  |],
  () =>
  switch (ReactDOM.querySelector("#schoolrouter-innerpage")) {
  | Some(root) =>
    let (courseId, authors) =
      DomUtils.parseJSONTag(~id="schools-courses-authors__props", ())
      |> decodeProps;
    let root = ReactDOM.Client.createRoot(root);
    ReactDOM.Client.render(root, <CourseAuthors__Root courseId authors />);
  | None => ()
  }
);
