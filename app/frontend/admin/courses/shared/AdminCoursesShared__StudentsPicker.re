let str = React.string;
let t = I18n.t(~scope="components.AdminCoursesShared__StudentsPicker");
type nonrec state = {
  students: array(UserProxy.t),
  search: string,
  loading: bool,
};
type nonrec action =
  | SetBaseData(array(UserProxy.t))
  | SetLoading
  | ClearLoading
  | SetSearch(string)
  | ClearSearch;
let reducer = (state, action) =>
  switch (action) {
  | SetBaseData(students) => {...state, students, loading: false}
  | SetLoading => {...state, loading: true}
  | ClearLoading => {...state, loading: false}
  | SetSearch(search) => {...state, search}
  | ClearSearch => {...state, search: ""}
  };
let computeInitialState = () => {loading: false, students: [||], search: ""};
module Selectable = {
  type nonrec t = UserProxy.t;
  let value = t => UserProxy.name(t);
  let searchString = value;
};
module StudentsPicker = MultiselectInline.Make(Selectable);
module StudentsPickerInfoQuery = [%graphql
  {js|
    query StudentsPickerInfoQuery($courseId: ID!, $filterString: String!) {
      courseStudents(courseId: $courseId, filterString: $filterString, first: 100 ) {
        nodes {
          id,
          user {
            id
            name
            avatarUrl
            fullTitle
          }
        }
      }
    }
  |js}
];
let loadStudentsData = (courseId, cohort, send) => {
  send(SetLoading);
  let filterString =
    Webapi.Url.URLSearchParams.makeWithArray([|
      ("cohort", Cohort.id(cohort) ++ {js|;|js}),
      ("not_teamed_up", "true"),
      ("status", "active"),
    |])
    ->Webapi.Url.URLSearchParams.toString;
  StudentsPickerInfoQuery.fetch(
    StudentsPickerInfoQuery.makeVariables(~courseId, ~filterString, ()),
  )
  |> Js.Promise.then_((response: StudentsPickerInfoQuery.t) => {
       send(
         SetBaseData(
           response.courseStudents.nodes
           ->(
               Js.Array.map(
                 ~f=(s: StudentsPickerInfoQuery.t_courseStudents_nodes) =>
                 UserProxy.make(
                   ~id=s.id,
                   ~name=s.user.name,
                   ~avatarUrl=s.user.avatarUrl,
                   ~fullTitle=s.user.fullTitle,
                   ~userId=s.user.id,
                 )
               )
             ),
         ),
       );
       Js.Promise.resolve();
     })
  |> ignore;
};
let handleCallBack = (send, callBack, student) => {
  callBack(student);
  send(ClearSearch);
};
let unselected = (students, selectedStudents) => {
  let selectedStudentIds =
    selectedStudents->(Js.Array.map(~f=s => UserProxy.id(s)));
  students->(
              Js.Array.filter(~f=s =>
                !Array.mem(UserProxy.id(s), selectedStudentIds)
              )
            );
};
[@react.component]
let make = (~courseId, ~selectedStudents, ~cohort, ~onSelect, ~onDeselect) => {
  let (state, send) = React.useReducer(reducer, computeInitialState());
  React.useEffect2(
    () => {
      loadStudentsData(courseId, cohort, send);
      None;
    },
    (courseId, cohort),
  );
  <StudentsPicker
    placeholder="Select students"
    emptySelectionMessage={t("no_students_selected")}
    allItemsSelectedMessage={
                              if (ArrayUtils.isEmpty(state.students)) {
                                t("no_students_found");
                              } else {
                                t("all_students_selected");
                              }
                            }
    selected=selectedStudents
    unselected={unselected(state.students, selectedStudents)}
    onChange={_ => ()}
    value={state.search}
    onSelect={handleCallBack(send, onSelect)}
    onDeselect={handleCallBack(send, onDeselect)}
  />;
};
