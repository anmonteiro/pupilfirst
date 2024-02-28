module Course = CoursesCurriculum__Course;
module Student = CoursesCurriculum__Student;
module Target = CoursesCurriculum__Target;
module Level = CoursesCurriculum__Level;
module TargetGroup = CoursesCurriculum__TargetGroup;
module LatestSubmission = CoursesCurriculum__LatestSubmission;
type nonrec lockReason =
  | CourseLocked
  | AccessLocked
  | LevelLocked(string)
  | PrerequisitesIncomplete;
type nonrec status =
  | Pending
  | PendingReview
  | Completed
  | Rejected
  | Locked(lockReason);
type nonrec t = {
  targetId: string,
  status,
};
let tc = I18n.t(~scope="components.CoursesCurriculum__TargetStatus");
type nonrec submissionStatus =
  | SubmissionMissing
  | SubmissionPendingReview
  | SubmissionCompleted
  | SubmissionRejected;
type nonrec cachedTarget = {
  targetId: string,
  targetReviewed: bool,
  levelNumber: int,
  milestone: bool,
  submissionStatus,
  prerequisiteTargetIds: array(string),
};
let isPast = date =>
  date->(Belt.Option.mapWithDefault(false, DateFns.isPast));
let makePending = targets =>
  targets |> Js.Array.map(~f=t => {targetId: t |> Target.id, status: Pending});
let lockTargets = (targets, reason) =>
  targets
  |> Js.Array.map(~f=t => {targetId: t |> Target.id, status: Locked(reason)});
let allTargetsComplete = (targetCache, targetIds) =>
  targetIds->(
               Belt.Array.every(targetId =>
                 Js.Array.find(~f=ct => ct.targetId == targetId, targetCache)
                 ->(
                     Belt.Option.mapWithDefault(true, target =>
                       target.submissionStatus == SubmissionCompleted
                     )
                   )
               )
             );
let compute =
    (preview, student, course, levels, targetGroups, targets, submissions) =>
  if (preview) {
    makePending(targets);
  } else if (course |> Course.ended) {
    lockTargets(targets, CourseLocked);
  } else if (student |> Student.endsAt |> isPast) {
    lockTargets(targets, AccessLocked);
  } else {
    let studentLevelNumber =
      levels
      |> ArrayUtils.unsafeFind(
           l => l |> Level.id == Student.levelId(student),
           "Could not student's level with ID " ++ Student.levelId(student),
         )
      |> Level.number;
    let targetCache =
      targets
      |> Js.Array.map(~f=target => {
           let targetId = target |> Target.id;
           let targetGroup =
             targetGroups
             |> ArrayUtils.unsafeFind(
                  tg => tg |> TargetGroup.id == Target.targetGroupId(target),
                  "Could not find target group with ID "
                  ++ Target.targetGroupId(target)
                  ++ " to create target cache",
                );
           let milestone = targetGroup |> TargetGroup.milestone;
           let levelNumber =
             levels
             |> ArrayUtils.unsafeFind(
                  l => l |> Level.id == (targetGroup |> TargetGroup.levelId),
                  "Could not find level with ID "
                  ++ Student.levelId(student)
                  ++ " to create target cache",
                )
             |> Level.number;
           let submission =
             submissions
             |> Js.Array.find(~f=s => s |> LatestSubmission.targetId == targetId);
           let submissionStatus =
             switch (submission) {
             | Some(s) =>
               if (s |> LatestSubmission.hasPassed) {
                 SubmissionCompleted;
               } else if (s |> LatestSubmission.hasBeenEvaluated) {
                 SubmissionRejected;
               } else {
                 SubmissionPendingReview;
               }
             | None => SubmissionMissing
             };
           {
             targetId,
             targetReviewed: target |> Target.reviewed,
             levelNumber,
             milestone,
             submissionStatus,
             prerequisiteTargetIds: Target.prerequisiteTargetIds(target),
           };
         });
    targetCache
    |> Js.Array.map(~f=ct => {
         let status =
           switch (ct.submissionStatus) {
           | SubmissionPendingReview => PendingReview
           | SubmissionCompleted => Completed
           | SubmissionRejected => Rejected
           | SubmissionMissing =>
             if (ct.levelNumber > studentLevelNumber && ct.targetReviewed) {
               Locked(LevelLocked(string_of_int(studentLevelNumber)));
             } else if (!(
                          ct.prerequisiteTargetIds
                          |> allTargetsComplete(targetCache)
                        )) {
               Locked(PrerequisitesIncomplete);
             } else {
               Pending;
             }
           };
         {targetId: ct.targetId, status};
       });
  };
let targetId = (t: t) => t.targetId;
let status = t => t.status;
let isPending = t => t.status == Pending;
let isAccessEnded = t =>
  switch (t.status) {
  | Locked(reason) => reason == AccessLocked
  | _ => false
  };
let lockReasonToString = lockReason =>
  switch (lockReason) {
  | CourseLocked => tc("course_locked")
  | AccessLocked => tc("access_locked")
  | LevelLocked(currentLevel) =>
    tc(~variables=[|("current_level", currentLevel)|], "level_locked")
  | PrerequisitesIncomplete => tc("prerequisites_incomplete")
  };
let statusToString = t =>
  switch (t.status) {
  | Pending => tc("status.pending")
  | PendingReview => tc("status.pending_review")
  | Completed => tc("status.completed")
  | Rejected => tc("status.rejected")
  | Locked(_) => tc("status.locked")
  };
let statusClassesSufix = t =>
  switch (t.status) {
  | Pending => "pending"
  | PendingReview => "pending-review"
  | Completed => "completed"
  | Rejected => "rejected"
  | Locked(_) => "locked"
  };
let canSubmit = (~resubmittable, t) =>
  switch (resubmittable, t.status) {
  | (true, Completed)
  | (_, Pending)
  | (_, Rejected) => true
  | (false, Completed)
  | (_, PendingReview)
  | (_, Locked(_)) => false
  };
let allAttempted = ts =>
  Belt.Array.every(ts, t =>
    Js.Array.includes(~value=t.status, [|PendingReview, Completed, Rejected|])
  );
let allComplete = ts => Belt.Array.every(ts, t => t.status == Completed);
let anyRejected = ts => Belt.Array.some(ts, t => t.status == Rejected);
