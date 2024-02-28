open CourseCoaches__Types;
let str = React.string;
let tr = I18n.t(~scope="components.CourseCoaches__EnrollmentForm");
let ts = I18n.t(~scope="shared");
type nonrec action =
  | UpdateCoachesList(array(string))
  | SelectCohort(Cohort.t)
  | DeSelectCohort(Cohort.t)
  | UpdateCoachSearchInput(string)
  | UpdateCohortSearchInput(string)
  | SetBaseData(array(Cohort.t))
  | SetLoading
  | ToggleSaving;
type nonrec state = {
  courseCoaches: array(string),
  cohorts: array(Cohort.t),
  selectedCohorts: array(Cohort.t),
  coachSearchInput: string,
  cohortSearchInput: string,
  loading: bool,
  saving: bool,
};
let reducer = (state, action) =>
  switch (action) {
  | UpdateCoachesList(courseCoaches) => {...state, courseCoaches}
  | ToggleSaving => {...state, saving: !state.saving}
  | UpdateCoachSearchInput(coachSearchInput) => {...state, coachSearchInput}
  | UpdateCohortSearchInput(cohortSearchInput) => {
      ...state,
      cohortSearchInput,
    }
  | SelectCohort(cohort) => {
      ...state,
      selectedCohorts: Js.Array.concat(state.selectedCohorts, ~other=[|cohort|]),
    }
  | DeSelectCohort(cohort) => {
      ...state,
      selectedCohorts:
        state.selectedCohorts
        ->(Js.Array.filter(~f=c => Cohort.id(c) != Cohort.id(cohort))),
    }
  | SetBaseData(cohorts) => {...state, cohorts, loading: false}
  | SetLoading => {...state, loading: true}
  };
let makePayload = state => {
  let payload = Js.Dict.empty();
  Js.Dict.set(
    payload,
    "authenticity_token",
    AuthenticityToken.fromHead() |> Js.Json.string,
  );
  Js.Dict.set(
    payload,
    "coach_ids",
    state.courseCoaches |> Json.Encode.(array(string)),
  );
  Js.Dict.set(
    payload,
    "cohort_ids",
    state.selectedCohorts->(Js.Array.map(~f=Cohort.id))
    |> Json.Encode.(array(string)),
  );
  payload;
};
module SelectableCourseCoaches = {
  type nonrec t = SchoolCoach.t;
  let value = t => t |> SchoolCoach.name;
  let searchString = value;
};
module SelectableCohort = {
  type nonrec t = Cohort.t;
  let value = t => t->Cohort.name;
  let searchString = value;
};
let setCoachSearchInput = (send, value) =>
  send(UpdateCoachSearchInput(value));
let selectCoach = (send, state, coach) => {
  let updatedCoaches =
    state.courseCoaches
    |> Js.Array.concat(~other=[|coach |> SchoolCoach.id|]);
  send(UpdateCoachesList(updatedCoaches));
};
let deSelectCoach = (send, state, coach) => {
  let updatedCoaches =
    state.courseCoaches
    |> Js.Array.filter(~f=coachId => coachId != SchoolCoach.id(coach));
  send(UpdateCoachesList(updatedCoaches));
};
module MultiselectForCourseCoaches =
  MultiselectInline.Make(SelectableCourseCoaches);
let courseCoachEditor = (coaches, state, send) => {
  let selected =
    coaches
    |> Js.Array.filter(~f=coach =>
         state.courseCoaches |> Array.mem(SchoolCoach.id(coach))
       );
  let unselected =
    coaches
    |> Js.Array.filter(~f=coach =>
         !(state.courseCoaches |> Array.mem(SchoolCoach.id(coach)))
       );
  <MultiselectForCourseCoaches
    placeholder={tr("search_coaches_placeholder")}
    emptySelectionMessage={tr("search_coaches_empty")}
    allItemsSelectedMessage={tr("search_coaches_all")}
    selected
    unselected
    onChange={setCoachSearchInput(send)}
    value={state.coachSearchInput}
    onSelect={selectCoach(send, state)}
    onDeselect={deSelectCoach(send, state)}
  />;
};
module MultiselectForCohorts = MultiselectInline.Make(SelectableCohort);
let courseCohortPicker = (cohorts, state, send) => {
  let selected = state.selectedCohorts;
  let selectedCohortIds = selected->(Js.Array.map(~f=s => Cohort.id(s)));
  let unselected =
    cohorts->(
               Js.Array.filter(~f=s =>
                 !Array.mem(Cohort.id(s), selectedCohortIds)
               )
             );
  <MultiselectForCohorts
    placeholder={tr("search_cohorts_placeholder")}
    emptySelectionMessage={tr("search_cohorts_empty")}
    allItemsSelectedMessage={tr("search_cohorts_all")}
    selected
    unselected
    onChange={value => send(UpdateCohortSearchInput(value))}
    value={state.cohortSearchInput}
    onSelect={s => send(SelectCohort(s))}
    onDeselect={s => send(DeSelectCohort(s))}
  />;
};
let handleResponseCB = (updateCoachesCB, json) => {
  let courseCoaches =
    json |> Json.Decode.(field("course_coaches", array(CourseCoach.decode)));
  updateCoachesCB(courseCoaches);
  Notification.success(
    ts("notifications.success"),
    tr("notification_coach_enrollment"),
  );
};
let updateCourseCoaches = (state, send, courseId, updateCoachesCB) => {
  send(ToggleSaving);
  let payload = makePayload(state);
  let url = "/school/courses/" ++ courseId ++ "/update_coach_enrollments";
  Api.create(url, payload, handleResponseCB(updateCoachesCB), () =>
    send(ToggleSaving)
  );
};
let computeAvailableCoaches = (schoolCoaches, courseCoaches) => {
  let courseCoachIds = courseCoaches |> Array.map(CourseCoach.id);
  schoolCoaches
  |> Js.Array.filter(~f=coach =>
       !(courseCoachIds |> Array.mem(coach |> SchoolCoach.id))
     );
};
module CohortFragment = Cohort.Fragment;
module CourseCoachesDataQuery = [%graphql
  {js|
  query CourseCoachesDataQuery($courseId: ID!) {
    course(id: $courseId) {
      cohorts {
        ...CohortFragment
      }
    }
  }
  |js}
];
let loadData = (courseId, send) => {
  send(SetLoading);
  CourseCoachesDataQuery.fetch(
    CourseCoachesDataQuery.makeVariables(~courseId, ()),
  )
  |> Js.Promise.then_((response: CourseCoachesDataQuery.t) => {
       send(
         SetBaseData(
           response.course.cohorts
           ->(Js.Array.map(~f=Cohort.makeFromFragment)),
         ),
       );
       Js.Promise.resolve();
     })
  |> ignore;
};
[@react.component]
let make = (~schoolCoaches, ~courseCoaches, ~courseId, ~updateCoachesCB) => {
  let (state, send) =
    React.useReducer(
      reducer,
      {
        courseCoaches: [||],
        cohorts: [||],
        loading: false,
        coachSearchInput: "",
        saving: false,
        cohortSearchInput: "",
        selectedCohorts: [||],
      },
    );
  React.useEffect1(
    () => {
      loadData(courseId, send);
      None;
    },
    [|courseId|],
  );
  let coaches = computeAvailableCoaches(schoolCoaches, courseCoaches);
  let saveDisabled =
    (state.courseCoaches->ArrayUtils.isEmpty || state.saving)
    || state.selectedCohorts->ArrayUtils.isEmpty;
  <div className="w-full">
    <div className="w-full">
      <div className="mx-auto bg-white">
        <div className="max-w-2xl pt-6 px-6 mx-auto">
          <h5
            className="uppercase text-center border-b border-gray-300 pb-2 mb-4">
            {tr("assign_coaches") |> str}
          </h5>
          {ReactUtils.nullIf(
             <div>
               <div id="course_coaches">
                 <span
                   className="inline-block mr-1 mb-2 text-xs font-semibold">
                   {(tr("select_coaches") ++ ":")->str}
                 </span>
                 {courseCoachEditor(coaches, state, send)}
               </div>
               <div>
                 <span
                   className="inline-block mr-1 mb-2 text-xs font-semibold">
                   {(tr("select_cohorts") ++ ":")->str}
                 </span>
                 {if (state.loading) {
                    <div className="max-w-5xl mx-auto px-2 mt-8">
                      {SkeletonLoading.input()}
                    </div>;
                  } else {
                    courseCohortPicker(state.cohorts, state, send);
                  }}
               </div>
             </div>,
             ArrayUtils.isEmpty(coaches),
           )}
        </div>
        <div className="flex max-w-2xl w-full mt-5 px-6 pb-5 mx-auto">
          <button
            disabled=saveDisabled
            onClick={_e =>
              updateCourseCoaches(state, send, courseId, updateCoachesCB)
            }
            className="w-full btn btn-primary btn-large">
            {tr("add_course") |> str}
          </button>
        </div>
      </div>
    </div>
  </div>;
};
