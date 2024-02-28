exception UnsafeFindFailed(string);
type nonrec courseId = string;
type nonrec id = string;
type nonrec coursePages =
  | Students
  | CourseCoaches
  | Curriculum
  | EvaluationCriteria
  | CourseExports
  | Authors
  | Certificates
  | Applicants
  | Teams
  | Cohorts;
type nonrec settingsPages =
  | Customization
  | Admins;
type nonrec t =
  | SchoolCoaches
  | Settings(settingsPages)
  | Courses
  | SelectedCourse(coursePages)
  | Communities;
let shrunk = t =>
  switch (t) {
  | SchoolCoaches
  | Courses
  | Communities => false
  | Settings(_settingsPages) => true
  | SelectedCourse(_coursePages) => true
  };
let isSPA = t =>
  switch (t) {
  | SchoolCoaches
  | Communities => false
  | Settings(_settingsPages) => false
  | SelectedCourse(coursePages) =>
    switch (coursePages) {
    | Cohorts
    | Students
    | Teams => true
    | CourseCoaches
    | Curriculum
    | EvaluationCriteria
    | CourseExports
    | Authors
    | Certificates
    | Applicants => false
    }
  | Courses => true
  };
let useSPA = (selectedPage, page) => isSPA(selectedPage) && isSPA(page);
let coursePath = (coursePage, courseId) =>
  switch (coursePage) {
  | Students =>
    ({js|/school/courses/|js} ++ courseId) ++ {js|/students?status=Active|js}
  | CourseCoaches => ({js|/school/courses/|js} ++ courseId) ++ {js|/coaches|js}
  | Curriculum => ({js|/school/courses/|js} ++ courseId) ++ {js|/curriculum|js}
  | EvaluationCriteria =>
    ({js|/school/courses/|js} ++ courseId) ++ {js|/evaluation_criteria|js}
  | CourseExports => ({js|/school/courses/|js} ++ courseId) ++ {js|/exports|js}
  | Authors => ({js|/school/courses/|js} ++ courseId) ++ {js|/authors|js}
  | Certificates =>
    ({js|/school/courses/|js} ++ courseId) ++ {js|/certificates|js}
  | Applicants => ({js|/school/courses/|js} ++ courseId) ++ {js|/applicants|js}
  | Teams =>
    ({js|/school/courses/|js} ++ courseId) ++ {js|/teams?status=Active|js}
  | Cohorts =>
    ({js|/school/courses/|js} ++ courseId) ++ {js|/cohorts?status=Active|js}
  };
let path = (~courseId=?, t) =>
  switch (t) {
  | SchoolCoaches => "/school/coaches"
  | Settings(settingsPages) =>
    switch (settingsPages) {
    | Customization => "/school/customize"
    | Admins => "/school/admins"
    }
  | Courses => "/school/courses"
  | SelectedCourse(coursePage) =>
    courseId->(
                Belt.Option.mapWithDefault("#", id =>
                  coursePath(coursePage, id)
                )
              )
  | Communities => "/school/communities"
  };
let name = t =>
  switch (t) {
  | SchoolCoaches => "Coaches"
  | Settings(settingsPages) =>
    switch (settingsPages) {
    | Customization => "Customization"
    | Admins => "Admins"
    }
  | Courses => "Courses"
  | SelectedCourse(coursePages) =>
    switch (coursePages) {
    | Students => "Students"
    | CourseCoaches => "Coaches"
    | Curriculum => "Curriculum"
    | EvaluationCriteria => "Evaluation Criteria"
    | CourseExports => "Exports"
    | Authors => "Authors"
    | Certificates => "Certificates"
    | Applicants => "Applicants"
    | Teams => "Teams"
    | Cohorts => "Cohorts"
    }
  | Communities => "Communities"
  };
let icon = t =>
  switch (t) {
  | SchoolCoaches => "users"
  | Settings(_settingsPages) => "cog"
  | Courses => "journal-text"
  | SelectedCourse(_coursePages) => "fas fa-book"
  | Communities => "comment-alt"
  };
