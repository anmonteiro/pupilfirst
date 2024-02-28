exception UnsafeFindFailed(string);
type nonrec courseId = string;
type nonrec id = string;
type nonrec coursePages =
  | Curriculum
  | Report
  | Review
  | Students
  | Leaderboard;
type nonrec t =
  | SelectedCourse(courseId, coursePages)
  | Student__SubmissionShow(id)
  | Student__StudentsReport(id);
let showSideNav = t =>
  switch (t) {
  | SelectedCourse(_id, _pages) => true
  | Student__StudentsReport(_id)
  | Student__SubmissionShow(_id) => false
  };
let coursePath = (id, page) =>
  (({js|/courses/|js} ++ id) ++ {js|/|js})
  ++ (
    switch (page) {
    | Curriculum => {js|curriculum|js}
    | Report => {js|report|js}
    | Review => {js|review|js}
    | Students => {js|students|js}
    | Leaderboard => {js|leaderboard|js}
    }
  );
let path = t =>
  switch (t) {
  | SelectedCourse(id, page) => coursePath(id, page)
  | Student__SubmissionShow(submissionId) =>
    ({js|/submissions/|js} ++ submissionId) ++ {js|/review|js}
  | Student__StudentsReport(studentId) =>
    ({js|/students/|js} ++ studentId) ++ {js|/report|js}
  };
let activeLinks = currentCourse => {
  let defaultLinks = [|Curriculum|];
  let linksForStudents =
    if (AppRouter__Course.isStudent(currentCourse)) {
      Js.Array.concat([|Report|], ~other=defaultLinks);
    } else {
      defaultLinks;
    };

  if (AppRouter__Course.canReview(currentCourse)) {
    Js.Array.concat([|Review, Students|], ~other=linksForStudents);
  } else {
    linksForStudents;
  };
};
let isSPA = t =>
  switch (t) {
  | SelectedCourse(_id, l) =>
    switch (l) {
    | Curriculum
    | Report
    | Leaderboard => false
    | Review
    | Students => true
    }
  | Student__SubmissionShow(_)
  | Student__StudentsReport(_) => true
  };
let useSPA = (selectedPage, page) => isSPA(selectedPage) && isSPA(page);
let isSelectedCoursePage = (t, coursePage) =>
  switch (t) {
  | SelectedCourse(_, page) => page == coursePage
  | Student__StudentsReport(_)
  | Student__SubmissionShow(_) => false
  };
let canAccessPage = (coursePage, course) =>
  Belt.Option.isSome(
    activeLinks(course)->(Js.Array.find(~f=l => l == coursePage)),
  );
