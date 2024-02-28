[%%raw {js|import "./SchoolRouter.css"|js}];
exception UnknownPathEncountered(list(string));
open SchoolRouter__Types;
let str = React.string;
let findSelectedCourse = (courses, currentCourseId) =>
  courses->(Js.Array.find(~f=c => Course.id(c) == currentCourseId));
let findAndSetSelectedCourse = (setSelectedCourse, courses, currentCourseId) =>
  setSelectedCourse(_ => findSelectedCourse(courses, currentCourseId));
[@react.component]
let make = (~school, ~courses, ~currentUser) => {
  let (selectedCourse, setSelectedCourse) = React.useState(() => None);
  let url = ReasonReactRouter.useUrl();
  React.useEffect1(
    () => {
      switch (url.path) {
      | ["school", "courses", courseId, ..._tale] =>
        findAndSetSelectedCourse(setSelectedCourse, courses, courseId)
      | _ => setSelectedCourse(_ => None)
      };
      None;
    },
    [|url|],
  );
  let (selectedPage: Page.t, component) =
    switch (url.path) {
    | ["school", "coaches"] => (SchoolCoaches, None)
    | ["school", "customize"] => (Settings(Customization), None)
    | ["school", "communities"] => (Communities, None)
    | ["school", "admins"] => (Settings(Admins), None)
    | ["school"]
    | ["school", "courses"]
    | ["school", "courses", "new"] => (
        Courses,
        Some(<CourseEditor__Root school />),
      )
    | ["school", "courses", _courseId] => (
        Courses,
        Some(<CourseEditor__Root school />),
      )
    | ["school", "courses", _courseId, "details" | "images" | "actions"] => (
        Courses,
        Some(<CourseEditor__Root school />),
      )
    | ["school", "students", studentId, "details"] => (
        SelectedCourse(Students),
        Some(<StudentDetails__Root studentId />),
      )
    | ["school", "students", studentId, "actions"] => (
        SelectedCourse(Students),
        Some(<StudentActions__Root studentId />),
      )
    | ["school", "teams", studentId, "details"] => (
        SelectedCourse(Teams),
        Some(<TeamsDetails__Root studentId />),
      )
    | ["school", "teams", studentId, "actions"] => (
        SelectedCourse(Teams),
        Some(<TeamsActions__Root studentId />),
      )
    | ["school", "cohorts", cohortId, "details"] => (
        SelectedCourse(Cohorts),
        Some(<CohortsDetails__Root cohortId />),
      )
    | ["school", "cohorts", cohortId, "actions"] => (
        SelectedCourse(Cohorts),
        Some(<CohortsActions__Root cohortId />),
      )
    | ["school", "courses", courseId, ...tail] =>
      switch (findSelectedCourse(courses, courseId)) {
      | Some(_course) =>
        let (coursePage: Page.coursePages, courseComponent) =
          switch (tail) {
          | ["cohorts"] => (
              Cohorts,
              Some(<CohortsIndex__Root courseId search={url.search} />),
            )
          | ["cohorts", "new"] => (
              Cohorts,
              Some(<CohortsCreator__Root courseId />),
            )
          | ["students"] => (
              Students,
              Some(<StudentsIndex__Root courseId search={url.search} />),
            )
          | ["students", "new"] => (
              Students,
              Some(<StudentCreator__Root courseId />),
            )
          | ["students", "import"] => (
              Students,
              Some(<StudentBulkImport__Root courseId />),
            )
          | ["students", studentId, "details"] => (
              Students,
              Some(<StudentDetails__Root studentId />),
            )
          | ["students", studentId, "actions"] => (
              Students,
              Some(<StudentActions__Root studentId />),
            )
          | ["teams"] => (
              Teams,
              Some(<TeamsIndex__Root courseId search={url.search} />),
            )
          | ["teams", "new"] => (
              Teams,
              Some(<TeamsCreator__Root courseId />),
            )
          | ["inactive_students"] => (Students, None)
          | ["coaches"] => (CourseCoaches, None)
          | ["curriculum"] => (Curriculum, None)
          | ["targets", _targetId, "content" | "versions" | "details"] => (
              Curriculum,
              None,
            )
          | ["exports"] => (CourseExports, None)
          | ["applicants"] => (Applicants, None)
          | ["applicants", _applicantId, "details" | "actions"] => (
              Applicants,
              None,
            )
          | ["authors"] => (Authors, None)
          | ["authors", _authorId] => (Authors, None)
          | ["certificates"] => (Certificates, None)
          | ["evaluation_criteria"] => (EvaluationCriteria, None)
          | _ =>
            Rollbar.critical(
              "Unknown path encountered by school router: "
              ++ Js.Array.join(~sep="/", Array.of_list(url.path)),
            );
            raise(UnknownPathEncountered(url.path));
          };
        (SelectedCourse(coursePage), courseComponent);
      | None => (SelectedCourse(Students), Some(<ErrorState />))
      }
    | _ =>
      Rollbar.critical(
        "Unknown path encountered by school router: "
        ++ Js.Array.join(~sep="/", Array.of_list(url.path)),
      );
      raise(UnknownPathEncountered(url.path));
    };
  <SchoolRouter__CourseContext.Provider
    value=(
            {
              selectedCourse,
              setCourseId:
                findAndSetSelectedCourse(setSelectedCourse, courses),
            }: SchoolRouter__CourseContext.t
          )>
    <div className="antialiased flex h-screen overflow-hidden ">
      <div className="flex school-admin-navbar flex-shrink-0">
        {<SchoolRouter__LeftNav school courses selectedPage currentUser />}
      </div>
      <div className="flex flex-col flex-1">
        <SchoolRouter__TopNav courses currentUser />
        <div role="main" className="flex h-full overflow-y-auto">
          <SchoolRouter__LeftSecondaryNav
            currentUser
            selectedCourse
            selectedPage
          />
          <div
            id="schoolrouter-innerpage"
            className="flex-1 overflow-y-scroll bg-white">
            {switch (component) {
             | Some(page) => page
             | None => React.null
             }}
          </div>
        </div>
      </div>
    </div>
  </SchoolRouter__CourseContext.Provider>;
};
