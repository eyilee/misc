using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Entities;
using Unity.Jobs;
using Unity.Transforms;
using Unity.Collections;
using Unity.Mathematics;
using Random = UnityEngine.Random;

public class MovementSystem : JobComponentSystem
{
    struct MovementJob : IJobProcessComponentData<Position, Rotation, MoveSpeed>
    {
        public float topBound;
        public float bottomBound;
        public float leftBound;
        public float rightBound;
        public float deltaTime;

        public void Execute (ref Position position, [ReadOnly] ref Rotation rotation, [ReadOnly] ref MoveSpeed moveSpeed)
        {
            float3 value = position.Value;

            value += deltaTime * moveSpeed.Value * new float3 (moveSpeed.x, moveSpeed.y, 0f);

            if (value.x < leftBound)
            {
                value.x = rightBound;
            }

            if (value.x > rightBound)
            {
                value.x = leftBound;
            }

            if (value.y < bottomBound)
            {
                value.y = topBound;
            }

            if (value.y > topBound)
            {
                value.y = bottomBound;
            }

            position.Value = value;
        }
    }

    protected override JobHandle OnUpdate (JobHandle inputDeps)
    {
        MovementJob moveJob = new MovementJob
        {
            topBound = GameManager.GM.topBound,
            bottomBound = GameManager.GM.bottomBound,
            leftBound = GameManager.GM.leftBound,
            rightBound = GameManager.GM.rightBound,
            deltaTime = Time.deltaTime
        };

        JobHandle moveHandle = moveJob.Schedule (this, inputDeps);

        return moveHandle;
    }
}
