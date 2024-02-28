let str = React.string;
let t = I18n.t(~scope="components.StudentDetails__Root");
let ts = I18n.ts;
module Coach = UserProxy;
type nonrec student = {
  name: string,
  taggings: array(string),
  usetTaggings: array(string),
  title: string,
  affiliation: string,
  coachIds: array(string),
  cohort: Cohort.t,
  email: string,
};
module Editor = {
  type nonrec state = {
    student,
    coachSearchInput: string,
    saving: bool,
  };
  type nonrec action =
    | UpdateName(string)
    | AddTag(string)
    | RemoveTag(string)
    | UpdateCoachesList(array(string))
    | UpdateCoachSearchInput(string)
    | UpdateTitle(string)
    | UpdateAffiliation(string)
    | UpdateSaving(bool)
    | UpdateCohort(Cohort.t);
  let stringInputInvalid = s => String.length(s) < 2;
  let updateName = (send, name) => send(UpdateName(name));
  let updateTitle = (send, title) => send(UpdateTitle(title));
  let formInvalid = state =>
    state.student.name->stringInputInvalid
    || state.student.title->stringInputInvalid;
  let handleErrorCB = (send, ()) => send(UpdateSaving(false));
  let enrolledCoachIds = coaches =>
    coaches
    ->(Js.Array.filter(~f=((_, _, selected)) => selected == true))
    ->(Js.Array.map(~f=((key, _, _)) => key));
  module UpdateStudentDetailsQuery = [%graphql
    {js|
    mutation UpdateStudentDetailsQuery($id: ID!, $cohortId: ID!, $coachIds: [ID!]!, $name: String!, $title: String!, $affiliation: String, $taggings: [String!]!) {
      updateStudentDetails(id: $id, cohortId: $cohortId, coachIds: $coachIds, name: $name, title: $title, affiliation: $affiliation, taggings: $taggings) {
        success
      }
    }
  |js}
  ];
  let updateStudent = (studentId, courseId, state, send) => {
    send(UpdateSaving(true));
    let variables =
      UpdateStudentDetailsQuery.makeVariables(
        ~id=studentId,
        ~cohortId=Cohort.id(state.student.cohort),
        ~name=state.student.name,
        ~title=state.student.title,
        ~affiliation=state.student.affiliation,
        ~taggings=state.student.taggings,
        ~coachIds=state.student.coachIds,
        (),
      );
    UpdateStudentDetailsQuery.fetch(variables)
    |> Js.Promise.then_((result: UpdateStudentDetailsQuery.t) => {
         if (result.updateStudentDetails.success) {
           ReasonReactRouter.push(
             ({js|/school/courses/|js} ++ courseId) ++ {js|/students|js},
           );
         } else {
           send(UpdateSaving(false));
         };
         Js.Promise.resolve();
       })
    |> Js.Promise.catch(error => {
         Js.log(error);
         send(UpdateSaving(false));
         Js.Promise.resolve();
       })
    |> ignore;
  };
  let boolBtnClasses = selected => {
    let classes = "toggle-button__button";
    classes ++ (if (selected) {" toggle-button__button--active"} else {""});
  };
  let handleTeamCoachList = (schoolCoaches, state) => {
    let selectedTeamCoachIds = state.coachIds;
    schoolCoaches->(
                     Js.Array.map(~f=coach => {
                       let coachId = Coach.id(coach);
                       let selected =
                         selectedTeamCoachIds->(
                                                 Js.Array.findIndex(
                                                   ~f=selectedCoachId =>
                                                   coachId == selectedCoachId
                                                 )
                                               )
                         > (-1);
                       (Coach.id(coach), Coach.name(coach), selected);
                     })
                   );
  };
  module SelectablePrerequisiteTargets = {
    type nonrec t = Coach.t;
    let value = t => Coach.name(t);
    let searchString = value;
    let make = (coach): t => coach;
  };
  let setCoachSearch = (send, value) => send(UpdateCoachSearchInput(value));
  let selectCoach = (send, state, coach) => {
    let updatedCoaches =
      state.coachIds->(Js.Array.concat(~other=[|Coach.id(coach)|]));
    send(UpdateCoachesList(updatedCoaches));
  };
  let deSelectCoach = (send, state, coach) => {
    let updatedCoaches =
      state.coachIds
      ->(Js.Array.filter(~f=coachId => coachId != Coach.id(coach)));
    send(UpdateCoachesList(updatedCoaches));
  };
  module MultiselectForTeamCoaches =
    MultiselectInline.Make(SelectablePrerequisiteTargets);
  let teamCoachesEditor = (courseCoaches, state: state, send) => {
    let selected =
      courseCoaches
      ->(
          Js.Array.filter(~f=coach =>
            Array.mem(Coach.id(coach), state.student.coachIds)
          )
        )
      ->(Js.Array.map(~f=coach => SelectablePrerequisiteTargets.make(coach)));
    let unselected =
      courseCoaches
      ->(
          Js.Array.filter(~f=coach =>
            !Array.mem(Coach.id(coach), state.student.coachIds)
          )
        )
      ->(Js.Array.map(~f=coach => SelectablePrerequisiteTargets.make(coach)));
    <div className="mt-1">
      <MultiselectForTeamCoaches
        placeholder={t("search_coaches_placeholder")}
        emptySelectionMessage={t("search_coaches_empty")}
        allItemsSelectedMessage={t("search_coaches_all")}
        selected
        unselected
        onChange={setCoachSearch(send)}
        value={state.coachSearchInput}
        onSelect={selectCoach(send, state.student)}
        onDeselect={deSelectCoach(send, state.student)}
      />
    </div>;
  };
  let initialState = student => {
    student,
    coachSearchInput: "",
    saving: false,
  };
  let reducer = (state: state, action) =>
    switch (action) {
    | UpdateName(name) => {
        ...state,
        student: {
          ...state.student,
          name,
        },
      }
    | AddTag(tag) => {
        ...state,
        student: {
          ...state.student,
          taggings:
            state.student.taggings->(Js.Array.concat(~other=[|tag|])),
        },
      }
    | RemoveTag(tag) => {
        ...state,
        student: {
          ...state.student,
          taggings:
            state.student.taggings->(Js.Array.filter(~f=t => t != tag)),
        },
      }
    | UpdateCoachesList(coachIds) => {
        ...state,
        student: {
          ...state.student,
          coachIds,
        },
      }
    | UpdateCoachSearchInput(coachSearchInput) => {
        ...state,
        coachSearchInput,
      }
    | UpdateTitle(title) => {
        ...state,
        student: {
          ...state.student,
          title,
        },
      }
    | UpdateAffiliation(affiliation) => {
        ...state,
        student: {
          ...state.student,
          affiliation,
        },
      }
    | UpdateSaving(bool) => {...state, saving: bool}
    | UpdateCohort(cohort) => {
        ...state,
        student: {
          ...state.student,
          cohort,
        },
      }
    };
  module Selectable = {
    type nonrec t = Cohort.t;
    let id = t => Cohort.id(t);
    let name = t => Cohort.name(t);
  };
  module Dropdown = Select.Make(Selectable);
  [@react.component]
  let make =
      (
        ~studentId,
        ~courseId,
        ~student,
        ~avilableTags,
        ~courseCoaches,
        ~cohorts,
      ) => {
    let (state, send) = React.useReducer(reducer, initialState(student));
    <DisablingCover disabled={state.saving}>
      <div className="bg-white">
        <div className="max-w-4xl 2xl:max-w-5xl mx-auto px-4">
          <div className="pt-6">
            <label className="block text-sm font-medium" htmlFor="name">
              {t("name")->str}
            </label>
            <input
              autoFocus=true
              value={state.student.name}
              onChange={event =>
                updateName(send, React.Event.Form.target(event)##value)
              }
              className="appearance-none block w-full bg-white border border-gray-300 rounded py-2.5 px-3 mt-1 text-sm focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focusColor-500"
              id="name"
              type_="text"
              placeholder={t("student_name_placeholder")}
            />
            <School__InputGroupError
              message="Name must have at least two characters"
              active={state.student.name->stringInputInvalid}
            />
          </div>
          <div className="pt-6">
            <label
              className="inline-block tracking-wide text-sm font-medium"
              htmlFor="title">
              {t("title")->str}
            </label>
            <input
              value={state.student.title}
              onChange={event =>
                updateTitle(send, React.Event.Form.target(event)##value)
              }
              className="appearance-none block w-full bg-white border border-gray-300 rounded py-2.5 px-3 text-sm focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focusColor-500"
              id="title"
              type_="text"
              placeholder={t("title_placeholder")}
            />
            <School__InputGroupError
              message={t("title_error")}
              active={state.student.title->stringInputInvalid}
            />
          </div>
          <div className="pt-6">
            <label
              className="inline-block tracking-wide text-sm font-medium"
              htmlFor="affiliation">
              {t("affiliation")->str}
            </label>
            <span className="text-xs ml-1">
              {ts("optional_braces")->str}
            </span>
            <input
              value={state.student.affiliation}
              onChange={event =>
                send(
                  UpdateAffiliation(React.Event.Form.target(event)##value),
                )
              }
              className="appearance-none block w-full bg-white border border-gray-300 rounded py-2.5 px-3 mt-1 text-sm focus:outline-none focus:bg-white focus:border-transparent focus:ring-2 focus:ring-focusColor-500"
              id="affiliation"
              type_="text"
              placeholder={t("affiliation_placeholder")}
            />
          </div>
          <div className="pt-6 flex flex-col">
            <label
              className="inline-block tracking-wide text-sm font-medium"
              htmlFor="email">
              {t("select_a_cohort")->str}
            </label>
            <Dropdown
              placeholder={t("pick_a_cohort")}
              selectables=cohorts
              selected={Some(state.student.cohort)}
              onSelect={u => send(UpdateCohort(u))}
            />
          </div>
          <div className="pt-6">
            <div className="border-b pb-6 ">
              <span className="block tracking-wide text-sm font-medium">
                {t("personal_coaches")->str}
              </span>
              {teamCoachesEditor(courseCoaches, state, send)}
            </div>
          </div>
          {if (state.student.taggings->ArrayUtils.isNotEmpty) {
             <div className="pt-6">
               <div className="block tracking-wide text-sm font-medium">
                 {str(t("tags_applied_user") ++ ":")}
               </div>
               <div className="flex flex-wrap">
                 {state.student.usetTaggings
                  ->(
                      Js.Array.map(~f=tag =>
                        <div
                          className="bg-blue-100 text-blue-700 border border-blue-500 rounded-full px-3 py-2 mt-1 mr-1 text-xs text-gray-900"
                          key=tag>
                          {str(tag)}
                        </div>
                      )
                    )
                  ->React.array}
               </div>
             </div>;
           } else {
             React.null;
           }}
          <div className="pt-6">
            <div className="block tracking-wide text-sm font-medium">
              {str(t("tags_applied") ++ ":")}
            </div>
            <School__SearchableTagList
              unselectedTags={
                avilableTags->(
                                Js.Array.filter(~f=tag =>
                                  !Array.mem(tag, state.student.taggings)
                                )
                              )
              }
              selectedTags={state.student.taggings}
              addTagCB={tag => send(AddTag(tag))}
              removeTagCB={tag => send(RemoveTag(tag))}
              allowNewTags=true
            />
          </div>
          <div className="py-6">
            <button
              disabled={formInvalid(state)}
              onClick={_e => updateStudent(studentId, courseId, state, send)}
              className="w-full btn btn-large btn-primary">
              {t("update_student")->str}
            </button>
          </div>
        </div>
      </div>
    </DisablingCover>;
  };
};
type nonrec baseData = {
  student,
  cohorts: array(Cohort.t),
  tags: array(String.t),
  courseCoaches: array(Coach.t),
  courseId: string,
};
type nonrec state =
  | Unloaded
  | Loading
  | Loaded(baseData)
  | Errored;
module UserProxyFragment = Coach.Fragment;
module CohortFragment = Cohort.Fragment;
module StudentDetailsDataQuery = [%graphql
  {js|
  query StudentDetailsDataQuery($studentId: ID!) {
    student(studentId: $studentId) {
      taggings
      cohort {
        ...CohortFragment
      }
      user {
        name
        title
        affiliation
        taggings
        email

      }
      personalCoaches{
        id
      }
      course {
        id
        cohorts {
          ...CohortFragment
        }
        coaches {
          ...UserProxyFragment
        }
        studentTags
      }

    }
  }
  |js}
];
let loadData = (studentId, setState, setCourseId) => {
  setState(_ => Loading);
  StudentDetailsDataQuery.fetch(
    ~notifyOnNotFound=false,
    {studentId: studentId},
  )
  |> Js.Promise.then_((response: StudentDetailsDataQuery.t) => {
       setState(_ =>
         Loaded({
           student: {
             name: response.student.user.name,
             taggings: response.student.taggings,
             title: response.student.user.title,
             affiliation:
               Belt.Option.getWithDefault(
                 response.student.user.affiliation,
                 "",
               ),
             coachIds:
               response.student.personalCoaches
               ->(
                   Js.Array.map(
                     ~f=(c: StudentDetailsDataQuery.t_student_personalCoaches) =>
                     c.id
                   )
                 ),
             cohort: response.student.cohort->Cohort.makeFromFragment,
             usetTaggings: response.student.user.taggings,
             email: response.student.user.email,
           },
           courseId: response.student.course.id,
           cohorts:
             response.student.course.cohorts
             ->(Js.Array.map(~f=Cohort.makeFromFragment)),
           tags: response.student.course.studentTags,
           courseCoaches:
             response.student.course.coaches
             ->(Js.Array.map(~f=Coach.makeFromFragment)),
         })
       );
       setCourseId(response.student.course.id);
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(_error => {
       setState(_ => Errored);
       Js.Promise.resolve();
     })
  |> ignore;
};
let pageLinks = studentId => [|
  School__PageHeader.makeLink(
    ~href=({js|/school/students/|js} ++ studentId) ++ {js|/details|js},
    ~title=t("pages.details"),
    ~icon="fas fa-edit",
    ~selected=true,
    (),
  ),
  School__PageHeader.makeLink(
    ~href=({js|/school/students/|js} ++ studentId) ++ {js|/actions|js},
    ~title=t("pages.actions"),
    ~icon="fas fa-cog",
    ~selected=false,
    (),
  ),
|];
[@react.component]
let make = (~studentId) => {
  let (state, setState) = React.useState(() => Unloaded);
  let courseContext = React.useContext(SchoolRouter__CourseContext.context);
  React.useEffect1(
    () => {
      loadData(studentId, setState, courseContext.setCourseId);
      None;
    },
    [|studentId|],
  );

  switch (state) {
  | Unloaded
  | Loading => SkeletonLoading.coursePage()
  | Loaded(baseData) =>
    <div>
      <School__PageHeader
        exitUrl={
          ({js|/school/courses/|js} ++ baseData.courseId) ++ {js|/students|js}
        }
        title={(t("edit") ++ {js| |js}) ++ baseData.student.name}
        description={baseData.student.email}
        links={pageLinks(studentId)}
      />
      <Editor
        courseCoaches={baseData.courseCoaches}
        avilableTags={baseData.tags}
        student={baseData.student}
        cohorts={baseData.cohorts}
        studentId
        courseId={baseData.courseId}
      />
    </div>
  | Errored => <ErrorState />
  };
};
