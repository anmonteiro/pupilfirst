exception UnknownPathEncountered(list(string));
open AppRouter__Types;
let str = React.string;
let classNames = (default, trueClasses, falseClasses, bool) =>
  (default ++ " ") ++ (if (bool) {trueClasses} else {falseClasses});
[@react.component]
let make = (~school, ~courses, ~currentUser) => {
  let url = ReasonReactRouter.useUrl();
  let (component, selectedPage: Page.t) =
    switch (url.path) {
    | ["courses", courseId, "review"] => (
        <CoursesReview__Root
          courseId
          currentCoachId={Belt.Option.getWithDefault(
            User.coachId(User.defaultUser(currentUser)),
            "",
          )}
          courses
        />,
        SelectedCourse(courseId, Review),
      )
    | ["submissions", submissionId, "review"] => (
        <CoursesReview__SubmissionsRoot
          submissionId
          currentUser={User.defaultUser(currentUser)}
        />,
        Student__SubmissionShow(submissionId),
      )
    | ["courses", courseId, "students"] => (
        <CoursesStudents__Root courseId />,
        SelectedCourse(courseId, Students),
      )
    | ["students", studentId, "report"] => (
        <CoursesStudents__StudentOverlay
          studentId
          userId={User.id(User.defaultUser(currentUser))}
        />,
        Student__StudentsReport(studentId),
      )
    | _ =>
      Rollbar.critical(
        "Unknown path encountered by app router: "
        ++ Js.Array.join(~sep="/", Array.of_list(url.path)),
      );
      raise(UnknownPathEncountered(url.path));
    };
  <div className="md:h-screen md:flex bg-gray-50 overflow-hidden">
    {ReactUtils.nullUnless(
       <AppRouter__Nav school courses selectedPage currentUser />,
       Page.showSideNav(selectedPage),
     )}
    component
  </div>;
};
