let decodeProps = json =>
  Json.Decode.(
    field("currentCourseId", string, json),
    field("courses", array(CourseInfo.decode), json),
    field("additionalLinks", array(string), json),
    field("coverImage", optional(string), json),
  );
Psj.matchPaths(
  [|"courses/:id/curriculum", "courses/:id/report"|],
  () => {
    let (currentCourseId, courses, additionalLinks, coverImage) =
      DomUtils.parseJSONAttribute(~id="course-header-root", ())->decodeProps;
    switch (ReactDOM.querySelector("#course-header-root")) {
    | Some(root) =>
      let root = ReactDOM.Client.createRoot(root);
      ReactDOM.Client.render(
        root,
        <StudentCourse__Header
          currentCourseId
          courses
          additionalLinks
          coverImage
        />,
      );
    | None => ()
    };
  },
);
