package com.example.dronecontroller.screen.components

import androidx.compose.foundation.Canvas
import androidx.compose.foundation.border
import androidx.compose.foundation.layout.BoxWithConstraints
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.runtime.Composable
import androidx.compose.runtime.SideEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp

@Composable
fun GraphWithMemory(modifier: Modifier, value: Float, color: Color) {
    BoxWithConstraints(modifier = modifier) {
        val xPointsGap = 3.dp
        val maxPoints = maxWidth / xPointsGap
        var values by remember { mutableStateOf(listOf<Float>()) }

        SideEffect {
            values = when {
                values.size < maxPoints -> values + value
                else -> values.drop(1) + value
            }
        }

        Canvas(
            modifier = Modifier
                .fillMaxSize()
                .border(width = 1.dp, Color.Black)
        ) {
            val startX = 0f
            val intervalX = xPointsGap.toPx()
            val intervalY = size.height / 180

            // Draw x-axis
            drawLine(
                start = Offset(startX, size.height / 2),
                end = Offset(size.width, size.height / 2),
                color = Color.Black,
                strokeWidth = 2f
            )

            // Draw points and lines
            values.forEachIndexed { index, y ->
                val x = startX + index * intervalX
                val yCoord = size.height / 2 - y * intervalY
                drawCircle(
                    color = color,
                    center = Offset(x, yCoord),
                    radius = 3f
                )
                if (index < values.size - 1) {
                    val nextX = startX + (index + 1) * intervalX
                    val nextYCoord = size.height / 2 - values[index + 1] * intervalY
                    drawLine(
                        start = Offset(x, yCoord),
                        end = Offset(nextX, nextYCoord),
                        color = color,
                        strokeWidth = 2f
                    )
                }
            }
        }
    }
}